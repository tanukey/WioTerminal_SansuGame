#!/usr/bin/env python3
import struct
import sys

UF2_MAGIC_START0 = 0x0A324655
UF2_MAGIC_START1 = 0x9E5D5157
UF2_MAGIC_END = 0x0AB16F30
UF2_FLAG_FAMILYID_PRESENT = 0x00002000
SAMD51_FAMILY_ID = 0x55114460

def main():
    in_path, out_path, base_addr_str = sys.argv[1], sys.argv[2], sys.argv[3]
    base_addr = int(base_addr_str, 0)
    with open(in_path, "rb") as f:
        data = f.read()

    chunk_size = 256
    num_blocks = (len(data) + chunk_size - 1) // chunk_size

    with open(out_path, "wb") as out:
        for block_no in range(num_blocks):
            chunk = data[block_no * chunk_size: (block_no + 1) * chunk_size]
            chunk = chunk + b"\x00" * (chunk_size - len(chunk))
            header = struct.pack(
                "<IIIIIIII",
                UF2_MAGIC_START0,
                UF2_MAGIC_START1,
                UF2_FLAG_FAMILYID_PRESENT,
                base_addr + block_no * chunk_size,
                chunk_size,
                block_no,
                num_blocks,
                SAMD51_FAMILY_ID,
            )
            block = header + chunk + b"\x00" * (476 - chunk_size) + struct.pack("<I", UF2_MAGIC_END)
            assert len(block) == 512
            out.write(block)

    print(f"Wrote {num_blocks} blocks to {out_path}")

if __name__ == "__main__":
    main()
