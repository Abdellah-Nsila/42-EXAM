#!/bin/bash
# 🧪 VBC COMPREHENSIVE TESTER (Math Expression Calculator)
echo "🧪 VBC COMPREHENSIVE TESTER (Math Expression Calculator)"
echo "======================================================"
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo "📁 Testing solution at: $DIR"

# 🔹 Compile with test_main.c instead of standalone vbc
echo -n "📦 Compiling vbc with test_main.c... "
if gcc -Wall -Wextra -Werror "$DIR/test_main.c" "$DIR/vbc.c" -o "$DIR/vbc_test"; then
    echo "✓ Compilation successful"
else
    echo "❌ Compilation failed"
    exit 1
fi

echo
echo "🚀 Running tests..."
echo

# Run the compiled tester instead of solution binary
"$DIR/vbc_test"
