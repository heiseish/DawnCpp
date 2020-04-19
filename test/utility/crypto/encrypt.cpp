#include "utility/crypto/encrypt.hpp"
#include <string>
#include "gtest/gtest.h"
namespace Dawn::Utility {

class EncryptTest : public ::testing::Test {};

TEST_F(EncryptTest, sha256) {
    auto res = sha256("abc");
    EXPECT_EQ(
        res,
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

    res = sha256("");
    EXPECT_EQ(
        res,
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");

    res = sha256("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
    EXPECT_EQ(
        res,
        "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");

    res = sha256(
        "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmn"
        "opjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
    EXPECT_EQ(
        res,
        "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1");
}

TEST_F(EncryptTest, hmac_sha256) {
    auto res =
        hmac_sha256("key", "The quick brown fox jumps over the lazy dog");
    EXPECT_EQ(
        res,
        "f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8");
}

}  // namespace Dawn::Utility