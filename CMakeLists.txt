# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Configuração da extensão VSCode (não alterar) ===
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)

# Caminho para integração com a extensão do VSCode
set(picoVscode "${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake")
if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ======================================================

# Especifica a placa utilizada
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Importa o SDK manualmente (pico_sdk_import.cmake deve existir no projeto)
include(pico_sdk_import.cmake)

# Nome do projeto e linguagens suportadas
project(GKM_AD C CXX ASM)

# Inicializa o SDK
pico_sdk_init()

# Adiciona o executável com o(s) arquivo(s) fonte
add_executable(GKM_AD 
    GKM_AD.c
)

# Nome e versão do programa
pico_set_program_name(GKM_AD "GKM_AD")
pico_set_program_version(GKM_AD "0.1")

# Saída padrão via USB (UART desativado)
pico_enable_stdio_uart(GKM_AD 0)
pico_enable_stdio_usb(GKM_AD 1)

# Vincula bibliotecas padrão e periféricos
target_link_libraries(GKM_AD
    pico_stdlib
    hardware_adc
    hardware_pwm
)

# Inclui diretório atual no projeto
target_include_directories(GKM_AD PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Gera arquivos extras (UF2, bin, etc.)
pico_add_extra_outputs(GKM_AD)
