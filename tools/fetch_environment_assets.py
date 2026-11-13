"""Fetch the small CC0 PBR material set used by FS Lap's environment pass.

The files stay in data/environment (generated, not hand-edited).  Each material
is downloaded at 1K first so the GTX 1650 version stays within its texture-memory
budget; move to 2K selectively once the scene composition is stable.

Source: Poly Haven, CC0.  The API is queried each run so URLs are not copied into
the repository or allowed to go stale.
"""
import json
import os
from pathlib import Path
from urllib.request import Request, urlopen

import numpy as np
from PIL import Image


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "data" / "environment"
API = "https://api.polyhaven.com/files/{}"

# id, output prefix, API map name.  DirectX normal maps are correct for Unreal.
MATERIALS = {
    "asphalt": ("asphalt_01", {"base": "Diffuse", "normal": "nor_dx", "rough": "Rough"}),
    "concrete": ("concrete_floor_02", {"base": "Diffuse", "normal": "nor_dx", "rough": "Rough"}),
}


def choose(files, key):
    """Return the 1K JPG URL for a Poly Haven texture channel."""
    channel = files.get(key)
    if not channel:
        raise KeyError(f"missing {key}; available: {', '.join(files)}")
    for size in ("1k", "2k"):
        fmt = channel.get(size, {})
        for extension in ("jpg", "png"):
            if extension in fmt:
                return fmt[extension]["url"]
    raise KeyError(f"{key} has no usable JPG or PNG")


def download(url, target):
    if target.exists() and target.stat().st_size > 1024:
        print("kept", target.name)
        return
    print("download", target.name)
    with urlopen(Request(url, headers={"User-Agent": "FS-Lap-environment-pipeline/1.0"}), timeout=90) as response, open(target, "wb") as out:
        while block := response.read(1024 * 1024):
            out.write(block)


# colour kept and mean sRGB level of the graded base maps: city tarmac and paving are grey, the source photos are warm and brown
GRADES = {"asphalt": (0.25, 60), "concrete": (0.3, 122)}


def grade(raw, target, keep, level):
    """The pristine download stays as *_raw; the graded map is made from it again on every run."""
    image = np.asarray(Image.open(raw).convert("RGB"), "f4")
    grey = image.mean(2, keepdims=True)
    image = grey + (image - grey) * keep
    Image.fromarray(np.clip(image * level / image.mean(), 0, 255).astype("u1")).save(target, quality=92)
    print("graded", target.name)


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    manifest = {"source": "Poly Haven (CC0)", "materials": {}}
    for name, (asset, channels) in MATERIALS.items():
        with urlopen(Request(API.format(asset), headers={"User-Agent": "FS-Lap-environment-pipeline/1.0"}), timeout=30) as response:
            files = json.load(response)
        manifest["materials"][name] = {"asset": asset, "maps": {}}
        for output, channel in channels.items():
            url = choose(files, channel)
            suffix = Path(url.split("?", 1)[0]).suffix
            target = OUT / f"{name}_{output}{suffix}"
            if output == "base" and name in GRADES:
                raw = OUT / f"{name}_{output}_raw{suffix}"
                download(url, raw)
                grade(raw, target, *GRADES[name])
            else:
                download(url, target)
            manifest["materials"][name]["maps"][output] = target.name
    (OUT / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
