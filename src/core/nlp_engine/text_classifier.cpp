#include "text_classifier.hpp"

#include <string>

#include "core/nlp_engine/action/action_types.hpp"
#include "library/algorithm/string/kmp.hpp"
#include "utility/string/tolower.hpp"
namespace Algo = Dawn::Library::Algorithm;

namespace Dawn::Core {

/**
 * Compute the intent of text in O((text length + sum(intent lenth) )* num
 * intent)
 */
BaseActionType TextClassifierEngine::classify(const std::string& text) {
    auto text_cp = text;
    Utility::ToLower(text_cp);
    if (Algo::KMPAlgorithm::Contains(text_cp.c_str(), "news")) {
        return BaseActionType::News;
    } else if (Algo::KMPAlgorithm::Contains(text_cp.c_str(), "pokemon go")) {
        return BaseActionType::PokemonGo;
    } else if (Algo::KMPAlgorithm::Contains(text_cp.c_str(), "weather")) {
        return BaseActionType::Weather;
    }
    return BaseActionType::Unknown;
}

}  // namespace Dawn::Core
