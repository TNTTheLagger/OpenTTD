#!/usr/bin/env bash
set -e

# 1) Resolve project root
SCRIPT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="${SCRIPT_PATH}/.."
echo "Project root: ${PROJECT_ROOT}"

# Save and clear PSP environment so it doesn't affect native host build
echo "→ Saving and unsetting PSP environment variables"
SAVED_PSPDEV="$PSPDEV"
SAVED_PSPSDK="$PSPSDK"
unset PSPDEV
unset PSPSDK

echo "SAVED_PSPDEV=${SAVED_PSPDEV}"
echo "SAVED_PSPSDK=${SAVED_PSPSDK}"

# 2) Build host tools (native build)
echo "→ Building host tools (native build)"
mkdir -p "${PROJECT_ROOT}/build-host"
pushd "${PROJECT_ROOT}/build-host" > /dev/null

cmake "${PROJECT_ROOT}" \
  -DCMAKE_TOOLCHAIN_FILE="" \
  -DCMAKE_C_COMPILER="$(which gcc)" \
  -DCMAKE_CXX_COMPILER="$(which g++)" \
  -DCMAKE_C_FLAGS="" \
  -DCMAKE_CXX_FLAGS="" \
  -DOPTION_TOOLS_ONLY=ON

cmake --build . --parallel
popd > /dev/null

# Add host tools to PATH
HOST_TOOLS_DIR="${PROJECT_ROOT}/build-host/src"
export PATH="${HOST_TOOLS_DIR}:${PATH}"
echo "Added host tools to PATH: ${HOST_TOOLS_DIR}"

# 3) Restore PSP environment
echo "→ Restoring PSP environment variables"
if [ -n "${SAVED_PSPDEV}" ]; then
    export PSPDEV="${SAVED_PSPDEV}"
    echo "Restored PSPDEV=${PSPDEV}"
fi
if [ -n "${SAVED_PSPSDK}" ]; then
    export PSPSDK="${SAVED_PSPSDK}"
    echo "Restored PSPSDK=${PSPSDK}"
fi

# 4) Build for PSP (cross compile)
echo "→ Building for PSP"
mkdir -p "${PROJECT_ROOT}/build-psp"
pushd "${PROJECT_ROOT}/build-psp" > /dev/null

cmake "${PROJECT_ROOT}" \
  -DCMAKE_TOOLCHAIN_FILE="${PSPDEV}/psp/share/pspdev.cmake" \
  -DCMAKE_BUILD_TYPE=Release

cmake --build . --parallel
popd > /dev/null

echo "→ PSP Build complete!"
