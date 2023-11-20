#include "PlayfairCipher.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>

PlayfairCipher::PlayfairCipher(const std::string& key){
    setKey(key);
}

std::string encounteredletters{""};

void PlayfairCipher::setKey(const std::string& key){
    // store the original key
    key_ = key;

    // Append the alphabet
    key_.append("abcdefghijklmnopqrstuvwxyz");

    // Make sure the key is upper case
    
    std::transform(key_.begin(),key_.end(),key_.begin(),toupper);   //transform algorithm which transforms all char in string to upper case

    // Remove non-alpha characters
    auto iter = std::remove_if(key_.begin(),key_.end(),[](char& val){return !std::isalpha(val);});
    key_.erase(iter,key_.end());
    
    // Change J -> I
    std::transform(key_.begin(),key_.end(),key_.begin(),[](char& val){if(val=='J'){return 'I';} else {return val;}});
    
    // Remove duplicated letters
    auto iter2 = std::remove_if(key_.begin(),key_.end(),[=](char& val){
        if(encounteredletters.find(val)!=std::string::npos){
            return true;
        } else {
            encounteredletters += val; 
            return false;
        }});
    key_.erase(iter2,key_.end());

    // Store the coords of each letter and the playfair cipher key map

    for(std::size_t i{0};i<key_.size();i++){
        Str2CoordMap::value_type invcoords{std::pair<int,int> {(i/5)+1,(i%5)+1},key_[i]};
        Str2CharMap::value_type coords{key_[i],std::pair<int,int> {(i/5)+1,(i%5)+1}};

        playfairciphermap_.insert(coords);
        playfairciphermapinv_.insert(invcoords);
    }
    /*
    for ( auto p : playfairciphermap_ ){
        std::cout << p.first << ": " << p.second.first << ":" << p.second.second << std::endl;
    }

    for (auto p : playfairciphermapinv_){
        std::cout << p.first.first << ":" << p.first.second << " :" << p.second << std::endl;
    }
    */
    
}


std::string PlayfairCipher::applyCipher(const std::string& inputText,
                                        const CipherMode cipherMode) const {

    std::string output{""},modifiedinput{""},input{inputText};

    switch(cipherMode){
        case CipherMode::Encrypt:
            // Change J → I
            std::transform(input.begin(),input.end(),input.begin(),[](char& val){if(val=='J'){return 'I';} else {return val;}});
            std::cout << "after j->i: " << input << std::endl;
            // If repeated chars in a digraph add an X or Q if XX
            modifiedinput += input[0];
            for(std::size_t j{1};j<input.size();j++){
                if(input[j] == input[j-1]){
                    if(input[j-1] != 'X'){
                        modifiedinput += 'X';
                        modifiedinput += input[j];
                    } else {
                        modifiedinput += 'Q';
                        modifiedinput += input[j];
                    }
                } else {
                    modifiedinput += input[j];
                }
            }
            std::cout << "after double char correction: " << modifiedinput << std::endl;
            // if the size of input is odd, add a trailing Z
            if(modifiedinput.size()%2 != 0){
                modifiedinput += 'Z';
            }
            std::cout << "after Z: " << modifiedinput << std::endl;
            // Loop over the input in Digraphs
            for(std::size_t k{0};k<modifiedinput.size();k+=2){
                std::cout << "iteration var: " << k  << "chars: " << modifiedinput[k] << "+" << modifiedinput[k+1] << std::endl;
                char digraphchar1{modifiedinput[k]};
                char digraphchar2{modifiedinput[k+1]};

                // - Find the coords in the grid for each digraph

                auto iterindex1{playfairciphermap_.find(digraphchar1)};
                auto iterindex2{playfairciphermap_.find(digraphchar2)};
                std::pair<int,int> charcoord1{(*iterindex1).second};
                std::pair<int,int> charcoord2{(*iterindex2).second};

                std::cout << "coordinate char 1: " << charcoord1.first << ":" << charcoord1.second << std::endl;
                std::cout << "coordinate char 2: " << charcoord2.first << ":" << charcoord2.second << std::endl;

                // - Apply the rules to these coords to get 'new' coords

                if(charcoord1.first == charcoord2.first){
                    //same row rule
                    std::cout << "row rule" << std::endl;
                    if(charcoord1.second == 5){
                        charcoord1.second = 1;
                    } else {
                        charcoord1.second++;
                    }

                    if(charcoord2.second == 5){
                        charcoord2.second = 1;
                    } else {
                        charcoord2.second++;
                    }
                } else if (charcoord1.second == charcoord2.second) {
                    //same column rule
                    std::cout << "column rule" << std::endl;
                    if(charcoord1.first == 5){
                        charcoord1.first = 1;
                    } else {
                        charcoord1.first++;
                    }

                    if(charcoord2.first == 5){
                        charcoord2.first = 1;
                    } else {
                        charcoord2.first++;
                    }
                } else {
                    //rectangle rule
                    std::cout << "rectangle rule" << std::endl;
                    int temp{charcoord1.second};
                    charcoord1.second = charcoord2.second;
                    charcoord2.second = temp;
                }

                // - Find the letter associated with the new coords

                auto iterinvindex1{playfairciphermapinv_.find(charcoord1)};
                auto iterinvindex2{playfairciphermapinv_.find(charcoord2)};
                char newchar1{(*iterinvindex1).second};
                char newchar2{(*iterinvindex2).second};

                std::cout << "new coord 1: " << charcoord1.first << ":" << charcoord1.second << std::endl;
                std::cout << "new coord 2: " << charcoord2.first << ":" << charcoord2.second << std::endl;

                output += newchar1;
                output += newchar2;

                std::cout << "new characters: " << newchar1 << " " << newchar2 << std::endl;
            }
            break;
        case CipherMode::Decrypt:
            output = input;
            break;
    }
    // return the text
    return output;
}