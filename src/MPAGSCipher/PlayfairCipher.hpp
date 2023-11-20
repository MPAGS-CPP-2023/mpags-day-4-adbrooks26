#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include <string>
#include <vector>
#include <map>

#include "CipherMode.hpp"

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */

class PlayfairCipher {
    public:
    /**
     * \brief Create a new Playfair Cipher, converting the given string into the key
     *
     * \param key the string to convert into the key to be used in the cipher
     */
    explicit PlayfairCipher(const std::string& key);

    /**
     * \brief sets the key for the PlayFair Cipher
     *
     * \param key the string to convert into the key to be used in the cipher
     */
    void setKey(const std::string& key);

    /**
     * \brief Apply the cipher to the provided text
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher(const std::string& inputText,
                            const CipherMode cipherMode) const;

    private:
    /// The cipher key, essentially a constant shift to be applied
    std::string key_{""};
    /// 5x5 map of the playfair cipher characters in the form (character,coord)
    using Str2CharMap = std::map<char,std::pair<int,int>>;
    Str2CharMap playfairciphermap_;
    ///inverse of 5x5 map in the form (coord,character) -- used for reverse lookup when coord is known
    using Str2CoordMap = std::map<std::pair<int,int>,char>;
    Str2CoordMap playfairciphermapinv_;

};

#endif //PLAYFAIRCIPHER_HPP
