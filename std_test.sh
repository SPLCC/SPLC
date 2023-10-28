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
            echo ================ "$file" =================
            # Get the filename without the directory path and suffix
            filename=$(basename "$file" .spl)

            # Call bin/parser with the input file and redirect the output to a .out file
            bin/spl_parser "$file" > "$input_directory/tmp_$filename.out" 2>&1
            
            output=$(diff "$input_directory/$filename.out" "$input_directory/tmp_$filename.out")

            exit_code=$?

            if [ $exit_code -eq 0 ]; then
                echo "Passed."
            elif [ $exit_code -eq 1 ]; then
                echo "Different"
                echo $output
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