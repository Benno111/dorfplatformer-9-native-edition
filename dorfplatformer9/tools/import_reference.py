"""Import runtime data from the supplied Dorfplatformer9 reference bundle."""
import argparse
import base64
import json
from pathlib import Path
import re
import shutil
import unicodedata
import xml.etree.ElementTree as ET


def filename_part(name):
    normalized = unicodedata.normalize("NFKD", name).encode("ascii", "ignore").decode()
    return re.sub(r"[^a-z0-9]+", "-", normalized.lower()).strip("-") or "unnamed"


def import_reference(samples, output):
    project = json.loads((samples / "source/project.json").read_text(encoding="utf-8"))
    levels_path = samples / "documentation/port_reference/levels.json"
    levels = json.loads(levels_path.read_text(encoding="utf-8"))
    if [level["name"] for level in levels] != [f"1-{i}" for i in range(1, 7)]:
        raise ValueError("Expected the six Dorfplatformer9 campaign levels")
    targets = {target["name"]: (index, target)
               for index, target in enumerate(project["targets"], 1)}
    costumes = {
        "tile": ("Levels", range(2, 31)),
        "sky": ("Stage", range(1, 8)),
        "player": ("Player body", (1, 2, 3, 4, 5, 7, 8, 9, 10)),
        "player_face": ("Player out", range(2, 12)),
        "enemy": ("Enemy", (2, 6, 7, 8, 9, 10)),
        "guard": ("Boss", range(1, 4)),
        "dragon": ("Dragon", (1, 4)),
        "boss_guard": ("Final Boss", (1,)),
        "fireball": ("Ball", (1,)),
        "fire_button": ("Phone Controls", (5,)),
        "flag": ("fane", (1,)),
        "menu": ("Menu", (1,)),
        "ending": ("Intro/BadEnding", (1, 2)),
        "transition": ("Died/Pipie", range(1, 7)),
    }
    sounds = {
        "day": ("Teme Day", range(1, 8)),
        "power_sound": ("Power up", (2,)),
        "night": ("Teme N", range(1, 7)),
        "player_sound": ("Player", range(1, 5)),
        "pipe_sound": ("Died/Pipie", (2,)),
        "goal_sound": ("Fane+", (4,)),
        "fireball_sound": ("Ball", (1,)),
        "dragon_sound": ("Dragon", (1, 2)),
        "title_sound": ("Menu", (3,)),
    }
    output.mkdir(parents=True, exist_ok=True)

    manifest = output / "media.txt"
    old_files = set()
    if manifest.is_file():
        for line in manifest.read_text(encoding="utf-8").splitlines():
            fields = line.split()
            if len(fields) >= 4 and fields[0] in {"image", "sound"}:
                filename = fields[3]
                if re.fullmatch(r"[a-z0-9-]+\.(png|svg|wav)", filename):
                    old_files.add(filename)

    new_files = set()
    pending_manifest = output / "media.txt.tmp"
    with pending_manifest.open("w", encoding="utf-8") as media:
        for kind, collection, source_folder, groups in (
            ("image", "costumes", "images", costumes),
            ("sound", "sounds", "audio", sounds),
        ):
            for group, (target_name, indices) in groups.items():
                target_index, target = targets[target_name]
                for index in indices:
                    asset = target[collection][index - 1]
                    original = asset["md5ext"]
                    if Path(original).name != original:
                        raise ValueError(f"Invalid asset filename {original}")
                    source = samples / "assets" / source_folder / original
                    suffix = source.suffix
                    if kind == "image":
                        pivot_x, pivot_y, resolution = (asset[key] for key in
                                                       ("rotationCenterX", "rotationCenterY",
                                                        "bitmapResolution"))
                    embedded_png = None
                    if (group, index) in {("player", 7), ("player_face", 9)}:
                        # These Scratch costumes wrap a single PNG covering the entire SVG canvas.
                        # SDL_image's SVG decoder cannot draw embedded raster images.
                        svg = ET.parse(source).getroot()
                        image = svg.find(".//{http://www.w3.org/2000/svg}image")
                        href = image.attrib["{http://www.w3.org/1999/xlink}href"]
                        prefix, payload = href.split(",", 1)
                        if prefix != "data:image/png;base64":
                            raise ValueError(f"Expected an embedded PNG in {original}")
                        embedded_png = base64.b64decode(payload, validate=True)
                        resolution = float(image.attrib["width"]) / float(svg.attrib["width"])
                        pivot_x *= resolution
                        pivot_y *= resolution
                        suffix = ".png"
                    filename = (f"{target_index:02}-{filename_part(target_name)}-"
                                f"{kind}-{index:03}-{filename_part(asset['name'])}{suffix}")
                    if embedded_png is None:
                        shutil.copyfile(source, output / filename)
                    else:
                        (output / filename).write_bytes(embedded_png)
                    new_files.add(filename)
                    fields = [kind, group, index, filename]
                    if kind == "image":
                        fields.extend((pivot_x, pivot_y, resolution))
                    print(*fields, file=media)

    pending_manifest.replace(manifest)
    for filename in old_files - new_files:
        (output / filename).unlink(missing_ok=True)

    with (output / "levels.txt").open("w", encoding="utf-8") as data:
        for level in levels:
            objects = level["objects"]
            print(level["name"], *level["metadata"], len(objects), file=data)
            for item in objects:
                print(item["x"], item["y"], item["type"], file=data)
    (output / "ready").touch()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("samples", type=Path)
    parser.add_argument("output", type=Path)
    arguments = parser.parse_args()
    import_reference(arguments.samples, arguments.output)
