// Copyright (c) Microsoft. All rights reserved.
//
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//
// hello_world.cpp: Sample #1 - Hello World!!
//

#include "stdafx.h"
#include "carbon_test_console.h"
#include "speechapi_cxx_language_understanding_model.h"
#include "speechapi_cxx_session.h"
#include "speechapi_cxx_intent_recognizer.h"


constexpr auto g_speechSubscriptionKey = LR"(1f30c291f2474d39acfdf1d3bdf847c3)";
constexpr auto g_customSpeechSubscriptionKey = LR"(82f1f909b993459d88384a53891f98d3)";
constexpr auto g_customSpeechModelId = LR"(eb29f6e4-e97b-4157-8d3c-9d64a7b21a58)";

void CarbonTestConsole::Sample_HelloWorld()
{
    auto factory = SpeechFactory::FromSubscription(g_speechSubscriptionKey, m_regionId);
    auto recognizer = factory->CreateSpeechRecognizer();

    ConsoleWriteLine(L"Say something...");
    auto result = recognizer->RecognizeAsync().get();

    ConsoleWriteLine(L"You said:\n\n    '%ls'", result->Text.c_str());
}

void CarbonTestConsole::Sample_HelloWorld_WithEvents()
{
    auto factory = SpeechFactory::FromSubscription(g_speechSubscriptionKey, m_regionId);
    auto recognizer = factory->CreateSpeechRecognizer();

    recognizer->IntermediateResult += [&](const SpeechRecognitionEventArgs& e) {
        ConsoleWriteLine(L"IntermediateResult: text=%ls", e.Result.Text.c_str());
    };

    ConsoleWriteLine(L"Say something...");
    auto result = recognizer->RecognizeAsync().get();

    ConsoleWriteLine(L"You said:\n\n    '%ls'", result->Text.c_str());
}

void CarbonTestConsole::Sample_HelloWorld_PickEngine(const wchar_t* pszEngine) // L"Usp", L"Unidec", or L"Mock"
{
    auto factory = SpeechFactory::FromSubscription(g_speechSubscriptionKey, m_regionId);
    auto recognizer = factory->CreateSpeechRecognizer();
    auto session = Session::FromRecognizer(recognizer);

    std::wstring propertyName = std::wstring(L"__use") + std::wstring(pszEngine) + std::wstring(L"RecoEngine");
    session->Parameters[propertyName.c_str()] = true;

    recognizer->IntermediateResult += [&](const SpeechRecognitionEventArgs& e) {
        ConsoleWriteLine(L"IntermediateResult: text=%ls", e.Result.Text.c_str());
    };

    ConsoleWriteLine(L"Say something...");
    auto result = recognizer->RecognizeAsync().get();

    ConsoleWriteLine(L"You said:\n\n    '%ls'", result->Text.c_str());
}

void CarbonTestConsole::Sample_HelloWorld_Intent()
{
    constexpr auto subscriptionKey = LR"(ee52996d8f814c0aa77f7a415f81bd4c)";
    constexpr auto appId = LR"(6ad2c77d180b45a288aa8c442538c090)";
    constexpr auto region = LR"(westus2)";

    Sample_HelloWorld_Intent(subscriptionKey, appId, region);
}

void CarbonTestConsole::Sample_HelloWorld_Intent(const wchar_t* subscriptionKey, const wchar_t* appId, const wchar_t* region)
{
    auto endpoint = LR"(wss://speech.platform.bing.com/speech/recognition/interactive/cognitiveservices/v1?setflight=cognitiveservicesintent&format=simple&language=en-us)";
    auto factory = SpeechFactory::FromEndpoint(endpoint, g_speechSubscriptionKey);

    // Create an intent recognizer using microphone as audio input.
    auto recognizer = factory->CreateIntentRecognizer();

    // Create a LanguageUnderstandingModel associated with your LU application
    auto luisSubscriptionKey = L"YourLuisSubscriptionKey"; luisSubscriptionKey = subscriptionKey;
    auto luisAppId = L"YourLuisAppId"; luisAppId = appId;
    auto luisRegion = L"YourLuisEndpoint"; luisRegion = region;
    auto model = LanguageUnderstandingModel::FromSubscription(luisSubscriptionKey, luisAppId, luisRegion);

    // Add each intent you wish to recognize to the intent recognizer
    auto intentName1 = L"IntentNameFromLuisPortal"; intentName1 = L"Calendar.Add";
    auto intentName2 = L"IntentNameFromLuisPortal"; intentName2 = L"Communication.SendEmail";

    recognizer->AddIntent(L"1", model, intentName1);
    recognizer->AddIntent(L"some other id", model, intentName2);

    // Prompt the user to speak
    // wcout << L"Say something...\n";
    ConsoleWriteLine(L"Say something...");

    // Start recognition; will return the first result recognized
    auto result = recognizer->RecognizeAsync().get();

    // Check the reason returned
    if (result->Reason == Reason::Recognized)
    {
        // wcout << L"We recognized: " << result->Text << '\n';
        // wcout << L"IntentId=" << result->IntentId << '\n';
        // wcout << L"json=" << result->Properties[ResultProperty::LanguageUnderstandingJson].GetString();
        ConsoleWriteLine(L"We recognized: %s", result->Text.c_str());
        ConsoleWriteLine(L"IntentId='%s'", result->IntentId.c_str());
        ConsoleWriteLine(L"json='%s'", result->Properties[ResultProperty::LanguageUnderstandingJson].GetString().c_str());
    }
    else if (result->Reason == Reason::NoMatch)
    {
        // wcout << L"We didn't hear anything" << '\n';
        ConsoleWriteLine(L"We didn't hear anything");
    }
    else if (result->Reason == Reason::Canceled)
    {
        // wcout << L"There was an error, reason " << int(result->Reason) << L"-" << result->Text << '\n';
        ConsoleWriteLine(L"There was an error, reason=%d - %s", int(result->Reason), result->Text.c_str());
    }
}

void CarbonTestConsole::Sample_HelloWorld_Subscription()
{
    auto factory = SpeechFactory::FromSubscription(m_subscriptionKey, m_regionId);
    auto recognizer = factory->CreateSpeechRecognizer();

    ConsoleWriteLine(L"Say something...");
    auto result = recognizer->RecognizeAsync().get();

    ConsoleWriteLine(L"You said:\n\n    '%ls'", result->Text.c_str());
}

void CarbonTestConsole::Sample_HelloWorld_Subscription_With_CRIS()
{
    auto factory = SpeechFactory::FromSubscription(g_customSpeechSubscriptionKey, m_regionId);
    auto recognizer = factory->CreateSpeechRecognizer();

    recognizer->SetDeploymentId(g_customSpeechModelId);

    ConsoleWriteLine(L"Say something...");
    auto result = recognizer->RecognizeAsync().get();

    ConsoleWriteLine(L"You said:\n\n    '%ls'", result->Text.c_str());
}

void CarbonTestConsole::Sample_HelloWorld_Kws()
{
    auto factory = SpeechFactory::FromSubscription(g_speechSubscriptionKey, m_regionId);
    auto recognizer = factory->CreateSpeechRecognizer();

    recognizer->IntermediateResult += [&](const SpeechRecognitionEventArgs& e) {
        ConsoleWriteLine(L"IntermediateResult: text=%ls", e.Result.Text.c_str());
    };
    recognizer->FinalResult += [&](const SpeechRecognitionEventArgs& e) {
        ConsoleWriteLine(L"FinalResult: text=%ls", e.Result.Text.c_str());
    };

    ConsoleWriteLine(L"Say something starting with \"Hey Cortana\", or press ENTER to quit.");

    auto model = KeywordRecognitionModel::FromFile(L"heycortana_en-US.table");
    recognizer->StartKeywordRecognitionAsync(model);

    std::wstring input;
    ConsoleReadLine(input);
}

void CarbonTestConsole::Sample_HelloWorld_Language(const wchar_t* language)
{
    auto factory = SpeechFactory::FromSubscription(g_speechSubscriptionKey, m_regionId);
    auto recognizer = factory->CreateSpeechRecognizer(language);

    ConsoleWriteLine(L"Say something...");
    auto result = recognizer->RecognizeAsync().get();

    ConsoleWriteLine(L"You said:\n\n    '%ls'", result->Text.c_str());
}

int CarbonTestConsole::Sample_Do_Speech(bool continuous)
{
    // auto factory = SpeechFactory::FromSubscription(g_speechSubscriptionKey);
    auto factory = !m_endpointUri.empty()
        ? SpeechFactory::FromEndpoint(m_endpointUri, m_subscriptionKey)
        : SpeechFactory::FromSubscription(m_subscriptionKey, m_regionId);

    auto recognizer = factory->CreateSpeechRecognizer();

    recognizer->IntermediateResult += [](const SpeechRecognitionEventArgs& e) {
        printf("INTERMEDIATE: %ls ...\n", e.Result.Text.c_str());
    };

    recognizer->FinalResult += [](const SpeechRecognitionEventArgs& e) {
        printf("FINAL RESULT: '%ls'\n", e.Result.Text.c_str());
    };

    if (continuous)
    {
        recognizer->StartContinuousRecognitionAsync();
        printf("Say something... (press ENTER to quit) \n");
        UNUSED(getchar());
        recognizer->StopContinuousRecognitionAsync();
    }
    else
    {
        printf("Say something... \n");
        recognizer->RecognizeAsync().get();
    }

    return 0;
}

int CarbonTestConsole::Sample_Do_Intent(bool continuous, const wchar_t* luisSubscription, const wchar_t* appId, const wchar_t* luisRegion)
{
    // auto factory = SpeechFactory::FromSubscription(g_speechSubscriptionKey); 
    auto factory = !m_endpointUri.empty()
        ? SpeechFactory::FromEndpoint(m_endpointUri, m_subscriptionKey)
        : SpeechFactory::FromSubscription(m_subscriptionKey, m_regionId);
    auto recognizer = factory->CreateIntentRecognizer();

    recognizer->IntermediateResult += [](const IntentRecognitionEventArgs& e) {
        printf("INTERMEDIATE: %ls ...\n", e.Result.Text.c_str());
    };

    recognizer->FinalResult += [](const IntentRecognitionEventArgs& e) {
        printf("FINAL RESULT: '%ls'\n", e.Result.Text.c_str());
        printf("   INTENT ID: '%ls'\n", e.Result.IntentId.c_str());
        printf("   LUIS JSON: '%ls'\n", e.Result.Properties[ResultProperty::LanguageUnderstandingJson].GetString().c_str());
    };

    auto model = LanguageUnderstandingModel::FromSubscription(luisSubscription, appId, luisRegion);
    recognizer->AddIntent(L"all intents", IntentTrigger::From(model, L"TV.ChangeChannel"));

    if (continuous)
    {
        recognizer->StartContinuousRecognitionAsync();
        printf("Say something... (press ENTER to quit) \n");
        UNUSED(getchar());
        recognizer->StopContinuousRecognitionAsync();
    }
    else
    {
        printf("Say something... \n");
        recognizer->RecognizeAsync().get();
    }

    return 0;
}

int CarbonTestConsole::Sample_Do_Intent(bool continuous)
{
    constexpr auto luisSubscription = LR"(ee52996d8f814c0aa77f7a415f81bd4c)";
    constexpr auto luisAppId = LR"(6ad2c77d180b45a288aa8c442538c090)";
    constexpr auto luisRegion = L"westus2";
    return Sample_Do_Intent(continuous, luisSubscription, luisAppId, luisRegion);
}

int CarbonTestConsole::Sample_Do_Intent_Kws(const wchar_t* luisSubscription, const wchar_t* luisAppId, const wchar_t* luisRegion)
{
    auto factory = !m_endpointUri.empty()
        ? SpeechFactory::FromEndpoint(m_endpointUri, m_subscriptionKey)
        : SpeechFactory::FromSubscription(m_subscriptionKey, m_regionId);
    auto recognizer = factory->CreateIntentRecognizer();

    recognizer->IntermediateResult += [](const IntentRecognitionEventArgs& e) {
        printf("INTERMEDIATE: %ls ...\n", e.Result.Text.c_str());
    };

    recognizer->FinalResult += [](const IntentRecognitionEventArgs& e) {
        printf("FINAL RESULT: '%ls'\n", e.Result.Text.c_str());
    };

    auto model = LanguageUnderstandingModel::FromSubscription(luisSubscription, luisAppId, luisRegion);
    recognizer->AddIntent(L"all intents", IntentTrigger::From(model, L"TV.ChangeChannel"));

    auto keywordModel = KeywordRecognitionModel::FromFile(L"heycortana_en-US.table");
    recognizer->StartKeywordRecognitionAsync(keywordModel);

    printf("Say 'Hey Cortana ... something' ... (press ENTER to quit) \n");
    UNUSED(getchar());

    return 0;
}

int CarbonTestConsole::Sample_Do_Intent_Kws()
{
    constexpr auto hostName = LR"(westus.api.cognitive.microsoft.com)";
    constexpr auto subscriptionKey = LR"(0415470b4d0d4ed7b736a7ccac71fa5d)";
    constexpr auto appId = LR"(52d44b1039734b78b5b54b2da0eaa6f5)";
    return Sample_Do_Intent_Kws(hostName, subscriptionKey, appId);
}

int channel9();

int CarbonTestConsole::Sample_Do_Channel9()
{
    return channel9();
}