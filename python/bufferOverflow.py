"""Buffer overflow detection. Create a buffer of a certain size and attempt to write beyond its limits.
   Use the script to target a specific vulnerable c binaary and observe the behavior when the buffer overflow occurs. This can help in understanding how buffer overflows work and how to prevent them in C programming.
"""

import ctypes
from pwn import *
# Set the target binary and the buffer size
binary_path = 'home/jerry/Downloads/./start'  # Replace with your vulnerable binary path
buffer_size = 50 # Size of the buffer in bytes
# Create a buffer of the specified size
buffer = ctypes.create_string_buffer(buffer_size)
# Attempt to write beyond the buffer limits
overflow_data = b'A' * (buffer_size + 10)  # Data that exceeds the buffer size
try:
    # Write the overflow data to the buffer
    ctypes.memmove(buffer, overflow_data, len(overflow_data))
    print("Buffer overflow attempted successfully.")
except Exception as e:
    print(f"An error occurred: {e}")

