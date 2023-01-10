#include "AES.h"



/// <summary>
/// function Initializes the AES class with the given key. The key should be an array of 16 std::uint8_t values.
/// </summary>
/// <param name="key"></param>
void AES::Init(const std::array<std::uint8_t, 16>& key)
{
	KeyExpansion(key);
}

/// <summary>
/// function is used  to rotate a word (32-bits) by one byte to the left
/// The first byte will be moved to the last position in the word, while the rest will be shifted by one position/
/// </summary>
/// <param name="word"></param>
/// <returns></returns>
std::uint32_t AES::RotateWord(std::uint32_t word)
{
    std::uint8_t a = (word >> 24) & 0xFF;
    std::uint8_t b = (word >> 16) & 0xFF;
    std::uint8_t c = (word >> 8) & 0xFF;
    std::uint8_t d = word & 0xFF;
    return (d << 24) | (a << 16) | (b << 8) | c;
}

/// <summary>
/// the function applies the substitution box (or "S-box") to each byte of the state array (encrypt)
/// </summary>
void AES::SubBytes()
{
    for (int i = 0; i < 16; ++i) 
    {
        state[i] = kSBox[state[i]];
    }
}

/// <summary>
/// the function applies the inverse S-box to each byte of the state array (decrypt)
/// </summary>
void AES::InvSubBytes()
{
    for (int i = 0; i < 16; ++i) 
    {
        state[i] = kInvSBox[state[i]];
    }
}

/// <summary>
///  function shifts the rows of the state array by a certain number of bytes to the LEFT (with the first row remaining unchanged). (encrypt)
/// </summary>
void AES::ShiftRows() 
{
    for (int i = 0; i < 4; ++i) 
    {
        // Shift the row by i bytes to the left.
        std::rotate(state.begin() + i * 4, state.begin() + i * 4 + i, state.begin() + i * 4 + 4);
    }
}


/// <summary>
/// function shifts the rows of the state array by a certain number of bytes to the RIGHT (with the first row remaining unchanged). (decrypt)
/// </summary>
void AES::InvShiftRows() 
{
    for (int i = 0; i < 4; ++i) 
    {
        // Shift the row by i bytes to the right.
        std::rotate(state.begin() + i * 4, state.begin() + i * 4 + 4 - i, state.begin() + i * 4 + 4);
    }
}


/// <summary>
/// function applies a linear transformation to the columns of the state array, effectively mixing their values. (encrypt)
/// </summary>
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

/// <summary>
/// function applies the inverse of the linear transformation of the encryption. (decrypt)
/// </summary>
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


/// <summary>
/// function takes the initial key and expands it into a key schedule, which is used in the subsequent encryption and decryption rounds.
/// </summary>
/// <param name="key"></param>
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


/// <summary>
/// Encrypts a block of 16 bytes of plaintext using the given key.
/// </summary>
/// <param name="plaintext"></param>
/// <returns></returns>
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

/// <summary>
/// Decrypts a block of 16 bytes of ciphertext using the given key.
/// </summary>
/// <param name="ciphertext"></param>
/// <returns></returns>
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