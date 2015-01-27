#ifndef DARKLEAKS_HPP
#define DARKLEAKS_HPP

#include <string>
#include <vector>

namespace darkleaks {

typedef std::vector<std::string> string_list;

/**
 * LEAKER
 * 1. Split file up into N chunks.
 * Returns number of chunks created.
 */
size_t start(
    const std::string document_filename,
    const std::string chunks_path,
    const size_t chunks);

struct prove_result_row
{
    size_t index;
    std::string pubkey;
};

typedef std::vector<prove_result_row> prove_result;

/**
 * LEAKER
 * 2. Prove authenticity of file through provably fair random selection
 *    of chunks.
 *    Reveal N of the chunks.
 */
prove_result prove(
    const std::string document_filename,
    const size_t chunks,
    const std::string block_hash,
    const size_t reveal);

/**
 * USER
 * 3. Verify file authenticity of file by unlocking revealed chunks.
 */
void unlock(
    const std::string chunk_filename,
    const std::string pubkey,
    const std::string decrypt_extension=".decrypted");

/**
 * LEAKER
 * 4. Claim Bitcoins and thereby unlock file.
 */
string_list secrets(
    const std::string document_filename,
    const size_t chunks);

} // namespace darkleaks

#endif

