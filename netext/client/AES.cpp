#include "AES.h"



// initiating the key_schedule field with the key itself so we wont need the key anymore
void AES::Init(const std::array<std::uint8_t, 16>& key)
{
	KeyExpansion(key);
}

// rotating a word (32 bytes)
std::uint32_t AES::RotateWord(std::uint32_t word)
{
    std::uint8_t a = (word >> 24) & 0xFF;
    std::uint8_t b = (word >> 16) & 0xFF;
    std::uint8_t c = (word >> 8) & 0xFF;
    std::uint8_t d = word & 0xFF;
    return (d << 24) | (a << 16) | (b << 8) | c;
}

// Performs the SubBytes step.
void AES::SubBytes()
{
    for (int i = 0; i < 16; ++i) 
    {
        state[i] = kSBox[state[i]];
    }
}

// Performs the InvSubBytes step.
void AES::InvSubBytes()
{
    for (int i = 0; i < 16; ++i) 
    {
        state[i] = kInvSBox[state[i]];
    }
}

// Performs the ShiftRows step.
void AES::ShiftRows() 
{
    for (int i = 0; i < 4; ++i) 
    {
        // Shift the row by i bytes to the left.
        std::rotate(state.begin() + i * 4, state.begin() + i * 4 + i, state.begin() + i * 4 + 4);
    }
}


// Performs the InvShiftRows step.
void AES::InvShiftRows() 
{
    for (int i = 0; i < 4; ++i) 
    {
        // Shift the row by i bytes to the right.
        std::rotate(state.begin() + i * 4, state.begin() + i * 4 + 4 - i, state.begin() + i * 4 + 4);
    }
}

void AES::MixColumns()
{
    std::array<std::uint8_t, 4> column;
    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 4; ++j) 
        {
            column[j] = state[i + j * 4];
        }
        state[i] = kMixColumnsMatrix[i + 0] * column[0] ^ kMixColumnsMatrix[i + 4] * column[1] ^ kMixColumnsMatrix[i + 8] * column[2] ^ kMixColumnsMatrix[i + 12] * column[3];
        state[i + 4] = kMixColumnsMatrix[i + 0] * column[1] ^ kMixColumnsMatrix[i + 4] * column[2] ^ kMixColumnsMatrix[i + 8] * column[3] ^ kMixColumnsMatrix[i + 12] * column[0];
        state[i + 8] = kMixColumnsMatrix[i + 0] * column[2] ^ kMixColumnsMatrix[i + 4] * column[3] ^ kMixColumnsMatrix[i + 8] * column[0] ^ kMixColumnsMatrix[i + 12] * column[1];
        state[i + 12] = kMixColumnsMatrix[i + 0] * column[3] ^ kMixColumnsMatrix[i + 4] * column[0] ^ kMixColumnsMatrix[i + 8] * column[1] ^ kMixColumnsMatrix[i + 12] * column[2];
    }
}

// Performs the InvMixColumns step.
void AES::InvMixColumns() 
{
    std::array<std::uint8_t, 4> column;
    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 4; ++j) 
        {
            column[j] = state[i + j * 4];
        }
        state[i] = kInvMixColumnsMatrix[i + 0] * column[0] ^ kInvMixColumnsMatrix[i + 4] * column[1] ^ kInvMixColumnsMatrix[i + 8] * column[2] ^ kInvMixColumnsMatrix[i + 12] * column[3];
        state[i + 4] = kInvMixColumnsMatrix[i + 0] * column[1] ^ kInvMixColumnsMatrix[i + 4] * column[2] ^ kInvMixColumnsMatrix[i + 8] * column[3] ^ kInvMixColumnsMatrix[i + 12] * column[0];
        state[i + 8] = kInvMixColumnsMatrix[i + 0] * column[2] ^ kInvMixColumnsMatrix[i + 4] * column[3] ^ kInvMixColumnsMatrix[i + 8] * column[0] ^ kInvMixColumnsMatrix[i + 12] * column[1];
        state[i + 12] = kInvMixColumnsMatrix[i + 0] * column[3] ^ kInvMixColumnsMatrix[i + 4] * column[0] ^ kInvMixColumnsMatrix[i + 8] * column[1] ^ kInvMixColumnsMatrix[i + 12] * column[2];
    }
}

// Performs the AddRoundKey step.
void AES::AddRoundKey(int round) 
{
    for (int i = 0; i < 16; ++i) {
        state[i] ^= key_schedule[i + round * 16];
    }
}

// Performs the KeyExpansion routine.
void AES::KeyExpansion(const std::array<std::uint8_t, 16>& key) 
{
    for (int i = 0; i < 16; ++i) 
    {
        key_schedule[i] = key[i];
    }
    std::uint32_t temp;
    for (int i = 4; i < 4 * (K_NUM_ROUNDS + 1); ++i) 
    {
        temp = key_schedule[i - 1];
        if (i % 4 == 0) 
        {
            temp = RotateWord(temp) ^ kRoundConstant[i / 4] ^ (kSBox[temp & 0xFF] << 24);
        }
        key_schedule[i] = key_schedule[i - 4] ^ temp;
    }
}


// Encrypts a block of 16 bytes of plaintext using the given key.
std::array<std::uint8_t, 16> AES::Encrypt(const std::array<std::uint8_t, 16>& plaintext) {

    state = plaintext;
    AddRoundKey(0);
    for (int round = 1; round <= K_NUM_ROUNDS; ++round) 
    {
        SubBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey(round);
    }
    return state;
}

// Decrypts a block of 16 bytes of ciphertext using the given key.
std::array<std::uint8_t, 16> AES::Decrypt(const std::array<std::uint8_t, 16>& ciphertext) {

    state = ciphertext;
    AddRoundKey(K_NUM_ROUNDS);
    for (int round = K_NUM_ROUNDS - 1; round >= 0; --round) 
    {
        InvMixColumns();
        InvShiftRows();
        InvSubBytes();
        AddRoundKey(round);
    }
    return state;
}