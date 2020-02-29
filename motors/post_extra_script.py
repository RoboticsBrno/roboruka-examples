import os
import hashlib
import subprocess
import json
import re
import shutil

Import("env", "projenv")

VERSION = 2 # increase to force spiffs flash

def generate_amalgamations(source, target, env):
    if not os.path.isdir("web"):
        return

    try:
        os.mkdir("data", 0o755)
    except OSError:
        pass

    fn_re = re.compile(r"^[0-9]+_.+\.([^.]+)$")
    to_combine = {}
    for root, dirs, files in os.walk("web"):
        for name in files:
            m = fn_re.match(name)
            path = os.path.join(root, name)
            if not m:
                shutil.copy(path, os.path.join("data", os.path.relpath(path, "web")))
            else:
                to_combine.setdefault(m.group(1), []).append(path)

    for ext, files in to_combine.items():
        files.sort(key=lambda p: os.path.basename(p))
        dst_path = os.path.join("data", "combined." + ext)
        print("Combining to %s:\n    %s" % (dst_path, "\n    ".join(files)))
        with open(dst_path, "wb") as dst:
            for path in files:
                 with open(path, "rb") as src:
                     shutil.copyfileobj(src, dst)
                     dst.write(b"\n")
env.AddPreAction("$BUILD_DIR/spiffs.bin", generate_amalgamations)

def after_upload(source, target, env):
    if not os.path.isdir("web"):
        return

    hasher = hashlib.sha1()
    hasher.update(str(VERSION).encode("utf-8"))

    for root, dirs, files in os.walk("web"):
        dirs.sort()
        for name in sorted(files):
            with open(os.path.join(root, name), "rb") as f:
                for chunk in iter(lambda: f.read(32787), b""):
                    hasher.update(chunk)

    try:
        with open("partitions.csv", "rb") as f:
            hasher.update(f.read())
    except OSError as e:
        pass

    dev_list = subprocess.check_output([ "pio", "device", "list", "--serial", "--json-output" ], env=env["ENV"])
    dev_list = json.loads(dev_list)
    for d in dev_list:
        hasher.update(d.get("hwid", "").encode("utf-8"))

    current_sha1 = hasher.hexdigest()
    if os.path.exists(".last_uploadfs_sha1"):
        with open(".last_uploadfs_sha1", "r") as f:
            if f.read() == current_sha1:
                print("SPIFFS data are the same.")
                return

    print("SPIFFS data changed, running uploadfs target!")

    env.Execute("pio run -t uploadfs")

    with open(".last_uploadfs_sha1", "w") as f:
        f.write(current_sha1)
env.AddPostAction("upload", after_upload)
