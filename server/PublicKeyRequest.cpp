#include "PublicKeyRequest.h"


PublicKeyRequest::PublicKeyRequest(std::array<char, 16>& id,
	char version, RequestCode code, unsigned int payload_size,std::string name, std::string key)
 : Request(id, version, code, payload_size),user_name(name), pubkey(key){
	
}


std::vector<char> PublicKeyRequest::serialize(){
    // Serialize the base class data using the base class method
    std::vector<char> baseData = Request::serialize();

    // Additional serialization for the name and public key
    std::vector<char> nameData(user_name.begin(), user_name.end());
    std::vector<char> keyData(pubkey.begin(), pubkey.end());


    // Combine the base class data and additional data
    baseData.insert(baseData.end(), nameData.begin(), nameData.end());
    baseData.insert(baseData.end(), keyData.begin(), keyData.end());
    return baseData;
}

