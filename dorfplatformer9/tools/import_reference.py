"""Import runtime data from the supplied Dorfplatformer9 reference bundle."""
import argparse
import json
from pathlib import Path
import shutil


def import_reference(samples, output):
    project = json.loads((samples / "source/project.json").read_text(encoding="utf-8"))
    levels_path = samples / "documentation/port_reference/levels.json"
    levels = json.loads(levels_path.read_text(encoding="utf-8"))
    if [level["name"] for level in levels] != [f"1-{i}" for i in range(1, 7)]:
        raise ValueError("Expected the six Dorfplatformer9 campaign levels")
    targets = {target["name"]: target for target in project["targets"]}
    costumes = {
        "tile": ("Levels", range(2, 31)),
        "sky": ("Stage", range(1, 8)),
        "player": ("Player body", range(1, 11)),
        "enemy": ("Enemy", (2, 3, 6, 7)),
        "guard": ("Boss", range(1, 4)),
        "dragon": ("Dragon", range(1, 5)),
        "flag": ("fane", (1,)),
        "menu": ("Menu", (1,)),
        "ending": ("Intro/BadEnding", (1, 2)),
        "transition": ("Died/Pipie", range(1, 7)),
    }
    sounds = {
        "day": ("Teme Day", range(1, 7)),
        "night": ("Teme N", range(1, 7)),
        "player_sound": ("Player", range(1, 5)),
        "pipe_sound": ("Died/Pipie", (2,)),
        "goal_sound": ("Fane+", (4,)),
        "dragon_sound": ("Dragon", (1, 2)),
        "title_sound": ("Menu", (3,)),
    }
    output.mkdir(parents=True, exist_ok=True)

    def copy(asset, subdirectory):
        filename = asset["md5ext"]
        if Path(filename).name != filename:
            raise ValueError(f"Invalid asset filename {filename}")
        shutil.copyfile(samples / "assets" / subdirectory / filename, output / filename)
        return filename

    with (output / "media.txt").open("w", encoding="utf-8") as media:
        for group, (target, indices) in costumes.items():
            for index in indices:
                costume = targets[target]["costumes"][index - 1]
                name = copy(costume, "images")
                print("image", group, index, name, costume["rotationCenterX"],
                      costume["rotationCenterY"], costume["bitmapResolution"], file=media)
        for group, (target, indices) in sounds.items():
            for index in indices:
                name = copy(targets[target]["sounds"][index - 1], "audio")
                print("sound", group, index, name, file=media)

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
