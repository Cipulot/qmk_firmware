import os
import sys
from pathlib import Path

QMK_PY = Path("lib") / "python"

sys.path.append(str(QMK_PY))

CWD = os.getcwd()
os.environ["QMK_HOME"] = os.environ["ORIG_CWD"] = CWD

from qmk.keyboard import list_keyboards
from qmk.build_targets import KeyboardKeymapBuildTarget

everything = list_keyboards()
cipulot = list(filter(lambda x: x.startswith("cipulot"), everything))

# remove previous artifacts
CWD = Path(CWD)
for ext in ("uf2", "bin", "hex"):
    for file in CWD.glob(f"*.{ext}"):
        file.unlink()

# actual compilation
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

        if target.compile() is not None:
            file.write(f" FAILED!\n")
        else:
            file.write(f" SUCCESS!\n")

        file.flush()
