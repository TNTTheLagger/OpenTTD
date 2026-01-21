cmake_minimum_required(VERSION 3.16)
project(OpenTTD_PSP C CXX)

# --- PSP Toolchain Setup ---
# If needed, this points CMake to your PSP toolchain file
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/psp.toolchain.cmake")
endif()

# Use C++20 (OpenTTD uses modern C++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- PSP Output & Options ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Optional: disable features that are not going to be used on PSP
add_compile_definitions(
    NO_LIBLZMA
    NO_LZO
    NO_ICU
    NO_FLUIDSYNTH
    NO_FONTCONFIG
)

# --- Sources ---
set(PSP_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/psp.cpp"
)


# --- PSP Executable Target ---
add_executable(openttd_psp ${PSP_SOURCES})

# --- Compile Definitions & Include Stub (if needed) ---
# Define PSP so your code sees the PSP build
target_compile_definitions(openttd_psp PRIVATE PSP)

# --- Link PSP Libraries ---
target_link_libraries(openttd_psp
        z
        png
        freetype
        harfbuzz
        SDL2
        GL
        pspctrl
        psppower
        pspdisplay
        pspge
        pspdebug
        pspkernel
)



# --- Include Paths ---
target_include_directories(openttd_psp PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/../.."            # root OpenTTD include base
        "${CMAKE_CURRENT_LIST_DIR}/../../src"       # core OpenTTD sources
        "${CMAKE_CURRENT_LIST_DIR}/../../src/os/psp" # PSP code
        "${PSPDEV}/psp/include"                      # PSP SDK headers
)


# --- Compile Flags for PSP ---
target_compile_options(openttd_psp PRIVATE
        -DPSP
        -fno-exceptions
        -fno-rtti
)

# --- Optional: Custom PSP build commands ---
# Example: create EBOOT.PBP for PSP
add_custom_command(TARGET openttd_psp POST_BUILD
    COMMAND psp-build-eboot ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/openttd_psp
    COMMENT "Generating PSP EBOOT"
)
