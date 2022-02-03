#include "Utilities.h"
#include "Serialization.h"

//Keyword logic from powerof3's CommonLibSSE implementation
bool Utilities::Keywords::AddKeyword(RE::TESForm* form, RE::BGSKeyword* newKeyword)
{
	const auto keywordForm = form->As<RE::BGSKeywordForm>();
	if (!keywordForm) {
		return false;
	}

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

	//Add keyword to saved keyword distribution
	Serialization::ArmorKeywordData::GetSingleton()->AppendData(form->formID, newKeyword->formID);

	return true;
}

void Utilities::Keywords::DistributeKeywords()
{
	const auto keywordData = Serialization::ArmorKeywordData::GetSingleton()->GetData();

	logger::info("Trying To Distribute {} Keywords", keywordData.size());

	std::map<RE::FormID, RE::BGSKeyword*> subFormCache;

	for (const auto& [formId, subForms] : keywordData) {
		logger::info("Trying to find formid {:X}", formId);
		auto form = RE::TESForm::LookupByID(formId);
		if (!form) {
			logger::info("Form is null");
			continue;
		}
		logger::info("Form is found");

		for (const auto subformId : subForms) {
			auto keywordSubForm = subFormCache[subformId];
			if (!keywordSubForm) {
				auto subForm = RE::TESForm::LookupByID(subformId);
				if (!subForm) {
					continue;
				}
				logger::info("subForm is found");

				keywordSubForm = subForm->As<RE::BGSKeyword>();
				if (!keywordSubForm) {
					continue;
				}

				subFormCache[subformId] = keywordSubForm;
			}
	
			logger::info("keywordSubForm is found");

			AddKeyword(form, keywordSubForm);
		}
	}
}
