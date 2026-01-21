#!/usr/bin/env bash
set -e

# 1) Resolve project root
SCRIPT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="${SCRIPT_PATH}/.."
echo "Project root: ${PROJECT_ROOT}"

# Save PSP-related environment
echo "→ Saving and unsetting PSP environment variables"
SAVED_PSPDEV="$PSPDEV"
SAVED_PSPSDK="$PSPSDK"
SAVED_CPATH="$CPATH"
SAVED_LIBRARY_PATH="$LIBRARY_PATH"
SAVED_PKG_CONFIG_PATH="$PKG_CONFIG_PATH"
SAVED_CMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH"

unset PSPDEV PSPSDK
unset CPATH LIBRARY_PATH PKG_CONFIG_PATH CMAKE_PREFIX_PATH

echo "Saved PSPDEV=${SAVED_PSPDEV}"

# 2) Build host tools (native build)
echo "→ Building host tools (native build)"
rm -rf "${PROJECT_ROOT}/build-host"
mkdir -p "${PROJECT_ROOT}/build-host"
pushd "${PROJECT_ROOT}/build-host" > /dev/null

cmake "${PROJECT_ROOT}" \
  -DOPTION_TOOLS_ONLY=ON \
  -DCMAKE_SYSTEM_NAME=Linux \
  -DCMAKE_C_COMPILER=/usr/bin/gcc \
  -DCMAKE_CXX_COMPILER=/usr/bin/g++ \
  -DCMAKE_FIND_ROOT_PATH= \
  -DCMAKE_PREFIX_PATH= \
  -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
  -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=NEVER \
  -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=NEVER

cmake --build . --parallel
popd > /dev/null

# Add host tools to PATH
HOST_TOOLS_DIR="${PROJECT_ROOT}/build-host/src"
export PATH="${HOST_TOOLS_DIR}:${PATH}"
echo "Added host tools to PATH: ${HOST_TOOLS_DIR}"

# 3) Restore PSP environment
echo "→ Restoring PSP environment variables"
[ -n "$SAVED_PSPDEV" ] && export PSPDEV="$SAVED_PSPDEV"
[ -n "$SAVED_PSPSDK" ] && export PSPSDK="$SAVED_PSPSDK"
[ -n "$SAVED_CPATH" ] && export CPATH="$SAVED_CPATH"
[ -n "$SAVED_LIBRARY_PATH" ] && export LIBRARY_PATH="$SAVED_LIBRARY_PATH"
[ -n "$SAVED_PKG_CONFIG_PATH" ] && export PKG_CONFIG_PATH="$SAVED_PKG_CONFIG_PATH"
[ -n "$SAVED_CMAKE_PREFIX_PATH" ] && export CMAKE_PREFIX_PATH="$SAVED_CMAKE_PREFIX_PATH"

# 4) Build for PSP (cross compile)
echo "→ Building for PSP"
rm -rf "${PROJECT_ROOT}/build-psp"
mkdir -p "${PROJECT_ROOT}/build-psp"
pushd "${PROJECT_ROOT}/build-psp" > /dev/null

cmake "${PROJECT_ROOT}" \
  -DCMAKE_TOOLCHAIN_FILE="${PSPDEV}/psp/share/pspdev.cmake" \
  -DCMAKE_BUILD_TYPE=Release \
  -DPSP=ON

cmake --build . --parallel
popd > /dev/null

echo "→ PSP Build complete!"
