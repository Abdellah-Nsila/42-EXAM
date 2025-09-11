#!/bin/bash

# Colores para output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'

# Configuración de directorios
EXERCISE="sandbox"
GRADEME_DIR="$(cd "$(dirname "$0")" && pwd)"
STUDENT_DIR="$(pwd)"   # <- use current dir
echo -e "${BLUE}🧪 SANDBOX COMPREHENSIVE TESTER${NC}"
echo -e "${BLUE}===================================${NC}"

# Verificar que existe el directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}❌ Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    echo -e "${YELLOW}💡 Crea tu solución: mkdir -p rendu/sandbox && cp your_sandbox.c rendu/sandbox/sandbox.c${NC}"
    exit 1
fi

# Verificar que existe el archivo del estudiante
if [ ! -f "$STUDENT_DIR/sandbox.c" ]; then
    echo -e "${RED}❌ Error: No se encuentra sandbox.c en $STUDENT_DIR${NC}"
    exit 1
fi

echo -e "${BLUE}📁 Testing solution at: $STUDENT_DIR/sandbox.c${NC}"
echo ""

# Crear directorio temporal
TEMP_DIR=$(mktemp -d)
trap 'rm -rf "$TEMP_DIR"' EXIT

# Copiar archivos necesarios
cp "$STUDENT_DIR"/* "$TEMP_DIR"
cp test_main.c "$TEMP_DIR"

# Ir al directorio temporal
cd "$TEMP_DIR"

# Crear una versión limpia del archivo del estudiante (sin main si lo tiene)
if grep -q "^int main(" sandbox.c; then
    echo -e "${YELLOW}ℹ️  Removing main function from student file for testing...${NC}"
    sed '/^int main(/,$d' sandbox.c > sandbox_clean.c
    mv sandbox_clean.c sandbox.c
fi

# Compilar
echo -e "${BLUE}📦 Compilando...${NC}"
gcc -Wall -Wextra -Werror test_main.c sandbox.c -o test_program 2>/dev/null

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Error de compilación${NC}"
    echo -e "${YELLOW}💡 Revisa que tu función sandbox esté correctamente implementada${NC}"
    echo -e "${YELLOW}💡 Asegúrate de que compile con -Wall -Wextra -Werror${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Compilación exitosa${NC}"
echo ""

# Ejecutar tests
echo -e "${BLUE}🚀 Ejecutando tests...${NC}"
echo ""

# Ejecutar el programa de test con timeout (más tiempo para sandbox porque usa timeouts internos)
timeout 30 ./test_program
test_result=$?

echo ""
if [ $test_result -eq 0 ]; then
    echo -e "${GREEN}🎉 ¡TODOS LOS TESTS PASARON!${NC}"
    echo -e "${GREEN}Tu implementación de sandbox funciona correctamente.${NC}"
    exit 0
elif [ $test_result -eq 124 ]; then
    echo -e "${RED}⏰ Tests timeout - posible bucle infinito o deadlock${NC}"
    exit 1
else
    echo -e "${RED}❌ Algunos tests fallaron.${NC}"
    echo -e "${YELLOW}💡 Revisa tu implementación y vuelve a intentar.${NC}"
    exit 1
fi