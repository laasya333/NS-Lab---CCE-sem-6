BEGIN {
    # This block is executed before processing any input
}

{
    # Process each input line
    for (i = 1; i <= NF; i++) {
        printf("col-%d: %s\n", i, $i);
    }
    printf("\n");
}

END {
    # This block is executed after processing all input
}
