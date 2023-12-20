#!/bin/bash

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Directory for negative test cases
negative_dir="test/negative"
# Directory for positive test cases
positive_dir="test/positive"

# Function to run the lexer on files in a given directory
run_lexer_on_dir() {
    for filepath in $1/*.txt; do
        echo -e "${BLUE}Running lexer on $filepath${NC}"
        ./lexer "$filepath"
        echo "----------------------------------"
    done
}

# Run the lexer on negative and positive test cases
echo -e "${RED}Running negative test cases...${NC}"
run_lexer_on_dir $negative_dir

echo -e "${GREEN}Running positive test cases...${NC}"
run_lexer_on_dir $positive_dir
