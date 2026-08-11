#include <stdio.h>
#include <string.h>

// Declare the external assembly function
extern void decode_and_reverse(char *buffer, size_t len, char key);

int main(void) {
    // Encrypted string: "pwn_time!" reversed and XORed with 0x42
    char secret[] = { 0x63, 0x27, 0x2B, 0x2B, 0x1F, 0x1D, 0x2C, 0x35, 0x32 };
    size_t len = sizeof(secret);
    char key = 0x42;

    printf("[+] Original raw bytes: ");
    for(size_t i = 0; i < len; i++) printf("0x%02X ", (unsigned char)secret[i]);
    printf("\n");

    // Call your NASM assembly function!
    decode_and_reverse(secret, len, key);

    printf("[+] Decoded Result: %s\n", secret);
    
    // Check if correct
    if (strcmp(secret, "!emit_nwp") == 0) { // "pwn_time!" reversed
        printf("[SUCCESS] You solved the assembly challenge!\n");
    } else {
        printf("[FAIL] Result did not match expected output.\n");
    }

    return 0;
}