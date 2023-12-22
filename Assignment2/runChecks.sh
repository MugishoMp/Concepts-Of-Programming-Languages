#!/bin/bash

# Directory for negative test cases
negative_dir="test/negative"
# Directory for positive test cases
positive_dir="test/positive"

# Function to run the lexer on files in a given directory
run_lexer_on_dir() {
    for filepath in $1/*.txt; do
        echo "Running lexer on $filepath"
        ./lexer "$filepath"
        echo "----------------------------------"
    done
}

# Run the lexer on negative and positive test cases
echo "Running negative test cases..."
run_lexer_on_dir $negative_dir

echo "Running positive test cases..."
run_lexer_on_dir $positive_dir
