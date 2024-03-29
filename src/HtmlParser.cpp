//
// Created by ameer on 9/15/19.
//

#include <iostream>
#include "HtmlParser.h"

HtmlParser::HtmlParser()
{
    letterMappings["%2B"] = "+";
    letterMappings["%3D"] = "=";
    letterMappings["%3F"] = "?";
    letterMappings["&amp;"] = "&";
    letterMappings["%252B"] = "+";
}

std::vector<QueryResult>& HtmlParser::parse(std::string source, std::string htmlPage)
{
    size_t pos = htmlPage.find(getTitlePattern(), 0);

    while (pos != std::string::npos)
    {
        int shift = getTitleShift();

        pos = pos + shift;

        int end_pos = htmlPage.find(getTitleEndPattern(), pos);

        std::string url = htmlPage.substr(pos, end_pos - pos);

        int posAmpersand = url.find("&amp;");

        url = url.substr(0, posAmpersand);

        url = encode(url);

        pos = end_pos;

        pos = htmlPage.find(getDescriptionPattern(), pos);

        pos = pos + getDescriptionShift();

        end_pos = htmlPage.find("<", pos);

        std::string desc = encode(htmlPage.substr(pos, end_pos - pos));

        queryResults.push_back(QueryResult(source, url, desc));

        pos = end_pos;

        pos = htmlPage.find(getTitlePattern(), pos);
    }

    return queryResults;
}

std::string HtmlParser::encode(std::string pattern)
{
    for (const auto &[key, value] : letterMappings)
    {
        pattern = ReplaceAll(pattern, key, value);
    }

    return pattern;
}
