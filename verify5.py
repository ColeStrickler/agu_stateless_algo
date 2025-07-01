# verify.py

import math

# === parameters ===
data_size = 4
j_max = k_max = l_max = 100
h_max = i_max = 10

# precompute strides (in “elements”, not bytes)
stride_l = 1
stride_k = l_max * stride_l
stride_j = k_max * stride_k
stride_i = j_max * stride_j
stride_h = i_max * stride_i
print(f"{stride_l}, {stride_k}, {stride_j}, {stride_i}, {stride_h}")


# build the map from “access index” → value (in bytes)
addr = {}
vals = {}
access = 0
for h in range(0, h_max):
    for i in range(0, i_max):
        for j in range(0, j_max):
            for k in range(0, k_max):
                for l in range(0, l_max):
                    addr[access] = i + j + k + l * data_size + (h*h)
                    vals[access] = f"h={h} i={i}, j={j}, k={k}, l={l}"
                    access += 1

# === the address we want to invert ===
base        = 640
target_addr = base + (64 * 130000 + 8)

# 1) compute the element‐index (0‐based) from the base‐adjusted address
offset = (target_addr - base) // data_size

# 2) peel off h, i, j, k, l
h_idx = offset // stride_h
offset %= stride_h

i_idx = offset // stride_i
offset %= stride_i

j_idx = offset // stride_j
offset %= stride_j

k_idx = offset // stride_k
offset %= stride_k

l_idx = offset // stride_l  # same as just offset

# if your original loops ran from 1..(max–1), convert back to 1-based:
i = i_idx 
j = j_idx 
k = k_idx 
l = l_idx 
h = h_idx

# 3) recompute the stored byte‐value and lookup the “actual” one
computed_value = i + j + k + l * data_size + (h*h)
actual_value   = addr[(target_addr - base) // data_size]

print(f"Decoded indices →  h={h}, i={i}, j={j}, k={k}, l={l}")
print(f"Actual {vals[(target_addr - base) // data_size]}")
print(f"Recomputed value = {computed_value} bytes")
print(f"Actual   value   = {actual_value} bytes")
