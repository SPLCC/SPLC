process_directory() {
    local input_directory="$1"

    # Check if the input directory exists
    if [ ! -d "$input_directory" ]; then
        echo "Directory '$input_directory' does not exist."
        return 1
    fi

    # Iterate over each .spl file in the directory
    for file in "$input_directory"/*.spl; do
        if [ -f "$file" ]; then
            printf '\x1b[33m'
            echo ================ "$file" =================
            printf '\x1b[0m'
            # Get the filename without the directory path and suffix
            filename=$(basename "$file" .spl)

            # Call bin/splc with the input file and redirect the output to a .out file
            bin/splc "$file" > "$input_directory/tmp_$filename.out" 2>&1
            
            output=$(diff "$input_directory/$filename.out" "$input_directory/tmp_$filename.out")

            exit_code=$?

            if [ $exit_code -eq 0 ]; then
                printf '\x1b[32m'
                echo "==>Passed."
                printf '\x1b[0m'
            elif [ $exit_code -eq 1 ]; then
                printf '\x1b[31m'
                echo "==>Difference found. Please check output files. "
                printf '\x1b[0m'
                echo
                echo "=========> Diff Output"
                
                echo $output
                echo
                echo "=========> Program Output"

                bin/splc "$file"
            else
                echo "An error occurred"
            fi
            echo
        fi
    done
}

# Check if an argument (directory path) is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <directory_path>"
    exit 1
fi

# Call the function with the provided directory path
process_directory "$1"