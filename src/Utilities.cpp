#include "Utilities.h"

//Keyword logic from powerof3's CommonLibSSE implementation
bool Utilities::Keywords::AddKeyword(RE::BGSKeywordForm* keywordForm, RE::BGSKeyword* newKeyword)
{
	//Try and find keyword in existing keyword array
	//If already exists return false
	if (keywordForm->keywords) {
		for (uint32_t i = 0; i < keywordForm->numKeywords; i++) {
			if (keywordForm->keywords[i] == newKeyword) {
				return false;
			}
		}
	}

	//Create new keywords array for keywordForm with added keyword
	//Copy existing keywords to vector
	std::vector<RE::BGSKeyword*> newKeywordsTemp(keywordForm->keywords, keywordForm->keywords + keywordForm->numKeywords);
	//Push back new keyword
	newKeywordsTemp.push_back(newKeyword);

	auto oldData = keywordForm->keywords;

	//Now copy newly formed vector back to form (need to explicitly allocate so memory stays when falls out of scope
	auto newKeywordsAlloc = RE::calloc<RE::BGSKeyword*>(newKeywordsTemp.size());
	std::copy(newKeywordsTemp.begin(), newKeywordsTemp.end(), newKeywordsAlloc);
	keywordForm->keywords = newKeywordsAlloc;
	keywordForm->numKeywords = static_cast<uint32_t>(newKeywordsTemp.size());

	//Free up old keyword data
	RE::free(oldData);

	return true;
}
