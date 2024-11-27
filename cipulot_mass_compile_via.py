import os
import sys
from pathlib import Path

QMK_PY = Path("lib") / "python"

sys.path.append(str(QMK_PY))

CWD = os.getcwd()
USERSPACE_VIA = os.path.abspath(os.path.join(CWD, "..", "qmk_userspace_via"))

os.environ["QMK_HOME"] = os.environ["ORIG_CWD"] = CWD

from qmk.keyboard import list_keyboards
from qmk.build_targets import KeyboardKeymapBuildTarget

everything = list_keyboards()
cipulot = list(filter(lambda x: x.startswith("cipulot"), everything))

# remove previous artifacts
sys.stdout.write("Removing previous artifacts in QMK Firmware folder...\n")
CWD = Path(CWD)
for ext in ("uf2", "bin", "hex"):
    for file in CWD.glob(f"*.{ext}"):
        file.unlink()

# remove previous artifacts
sys.stdout.write("Removing previous artifacts in QMK Userspace VIA folder...\n")
USERSPACE_VIA = Path(USERSPACE_VIA)
for ext in ("uf2", "bin", "hex"):
    for file in USERSPACE_VIA.glob(f"*.{ext}"):
        file.unlink()

# actual compilation
sys.stdout.write("Compiling Cipulot boards with VIA...\n")
with (CWD / "cipulot.log").open("w") as file:
    for board in cipulot:
        if board == "cipulot/common":
            continue

        compile_message = f"Compiling: {board} "
        dots = "." * (50 - len(compile_message))
        compile_message += dots

        file.write(compile_message)

        target = KeyboardKeymapBuildTarget(board, "via")
        target.configure(parallel=16)

        # Construct the correct keymap directory path
        keymap_directory = Path(USERSPACE_VIA) / "keyboards" / board / "keymaps" / "via"
        target.extra_args = {
            "MAIN_KEYMAP_PATH_1": str(keymap_directory),
            "MAIN_KEYMAP_PATH_2": str(keymap_directory),
            "MAIN_KEYMAP_PATH_3": str(keymap_directory),
            "MAIN_KEYMAP_PATH_4": str(keymap_directory),
            "MAIN_KEYMAP_PATH_5": str(keymap_directory),
        }

        if target.compile() is not None:
            file.write(f" FAILED!\n")
        else:
            file.write(f" SUCCESS!\n")

        file.flush()
