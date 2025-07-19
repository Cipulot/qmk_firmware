import os
import sys
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor, as_completed
import threading

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
def compile_board(board, userspace_via, log_lock):
    """Compile a single board and return the result"""
    try:
        target = KeyboardKeymapBuildTarget(board, "via")
        target.configure(parallel=16)

        # Construct the correct keymap directory path
        keymap_directory = Path(userspace_via) / "keyboards" / board / "keymaps" / "via"
        target.extra_args = {
            "MAIN_KEYMAP_PATH_1": str(keymap_directory),
            "MAIN_KEYMAP_PATH_2": str(keymap_directory),
            "MAIN_KEYMAP_PATH_3": str(keymap_directory),
            "MAIN_KEYMAP_PATH_4": str(keymap_directory),
            "MAIN_KEYMAP_PATH_5": str(keymap_directory),
        }

        compile_result = target.compile()

        compile_message = f"Compiling: {board} "
        dots = "." * (50 - len(compile_message))
        compile_message += dots

        if compile_result is not None:
            return (board, compile_message, "FAILED!")
        else:
            return (board, compile_message, "SUCCESS!")
    except Exception as e:
        compile_message = f"Compiling: {board} "
        dots = "." * (50 - len(compile_message))
        compile_message += dots
        return (board, compile_message, f"ERROR: {str(e)}")

sys.stdout.write("Compiling Cipulot boards with VIA...\n")

# Filter out cipulot/common before parallel processing
boards_to_compile = [board for board in cipulot if board != "cipulot/common"]

log_lock = threading.Lock()
results = []

# Use ThreadPoolExecutor for parallel compilation
# Adjust max_workers based on your system capabilities
# Consider memory usage and CPU cores when setting this value
max_parallel_builds = 4  # You can adjust this number based on your system

with ThreadPoolExecutor(max_workers=max_parallel_builds) as executor:
    # Submit all compilation tasks
    future_to_board = {
        executor.submit(compile_board, board, USERSPACE_VIA, log_lock): board
        for board in boards_to_compile
    }

    # Process completed tasks as they finish
    with (CWD / "cipulot.log").open("w") as file:
        for future in as_completed(future_to_board):
            board, compile_message, result = future.result()
            file.write(f"{compile_message} {result}\n")
            file.flush()
            sys.stdout.write(f"{compile_message} {result}\n")
            sys.stdout.flush()
