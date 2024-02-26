#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

class Graph 
{
public:
    vector<vector<int>> adj_list;
    Graph(int n) {
        adj_list.resize(n);
    }
    void add_edge(int u, int v) {
        adj_list[u].push_back(v);
    }

    void print_graph() {
        for (int i = 0; i < adj_list.size(); i++) {
            cout << i << " : ";
            for (int j = 0; j < adj_list[i].size(); j++) {
                cout << adj_list[i][j] << " ";
            }
            cout << endl;
        }
    }
    vector<int> get_adj_list(int s) 
    {
        return adj_list[s];
    }

};
void print(vector<string> vec)
{
    for (auto x : vec)
        cout << x << "     ";
    cout << endl;
}
void print(vector<vector<string>> arr)
{
    for (int i = 0; i < arr.size(); i++)
    {
        print(arr[i]);
    }
}
vector<string> split(string s, char delimiter) {
    vector<string> words;
    string word;
    int flag = true;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '\"')
        {
            i++;
            while (s[i] != '\"')
            {
                word += s[i];
                i++;
            }
            words.push_back(word);
            i++;
            word = ""; 
        }
        else
        {
            if (s[i] == delimiter) {
                words.push_back(word);
                word = "";
            }
            else {
                word += s[i];
            }
        }

    }
    if(word!="")
        words.push_back(word);
    return words;
}
vector<vector<string>> Get_Keywords(string fileName)
{
    vector<vector<string>> arr;
    int index = 0;
    ifstream fin;
    string line;
    fin.open(fileName);
    while (getline(fin, line, '#'))
        arr.push_back(split(line, ','));
    return arr;
}
string removespace(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == ' ')
        {
            s.erase(s.begin() + i);
            i--;
        }
    }
    return s;
}
vector<vector<int>> read_excel_file(string file_name) {
    vector<vector<int>> result;
    ifstream file(file_name);
    string line;
    while (getline(file, line)) 
    {
        stringstream ss(line);
        vector<int> temp;
        string word;
        while (getline(ss, word, ',')) {
            word = removespace(word);
            if (word.length() == 0)
                continue;
            temp.push_back(stoi(word));
        }
        result.push_back(temp);
    }
    return result;
}
vector<vector<double>> read_excel_file_2(string file_name) {
    vector<vector<double>> result;
    ifstream file(file_name);
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        vector<double> temp;
        string word;
        while (getline(ss, word, ',')) {
            word = removespace(word);
            if (word.length() == 0)
                continue;
            temp.push_back(stod(word));
        }
        result.push_back(temp);
    }
    return result;
}
void calculatePageRank(int n)
{
    //creating the graph
    Graph graph(n);
    ifstream in;
    string line;
    vector<string> pages;

    vector<vector<int>> arr = read_excel_file("Pages.csv");
    for (int i = 0; i < arr.size(); i++)
        for (int j = 1; j < arr[i].size(); j++)
            graph.add_edge(arr[i][0], arr[i][j]);


    vector<vector<double>> PageRank(n);
    for (int i = 0; i < n; i++)//Initilizaiton
    {
        PageRank[i].push_back(1.0 / n);
        PageRank[i].push_back(0);
    }
    double MIN = 1e9;//I will use it for normalization
    double MAX = 0;

    for (int i = 1; i < 101; i++) {
        bool achieved = true;
        for (int webs = 0; webs < n; webs++) {
            const double d = 0.85;//damping factor
            double cur = 0;
            for (int j : graph.get_adj_list(webs))
            {//all websites poinitng at webs
                cur += (PageRank[j][(i - 1) % 2] / graph.get_adj_list(j).size());
            }
            cur *= d;
            cur += (1 - d) / n;
            PageRank[webs][i % 2] = cur;
            MIN = min(cur, MIN);
            MAX = max(cur, MAX);
            achieved &= (abs(cur - PageRank[webs][(i - 1) % 2]) < 0.01);

        }
        if (achieved) 
        {
            if (i % 2 == 1) {//the answer in the second column, so we put it in the first column as well
                for (int i = 0; i < n; i++)
                {
                    if (MIN != MAX) {//that would be prbably always the case as we used damping factor
                        PageRank[i][0] = (PageRank[i][1] - MIN) / (MAX - MIN);//normalizing usin Min-Max Normalizaition
                    }
                }
            }
            for (int i = 0; i < PageRank.size(); i++) {
                cout << i << " : ";
                for (int j = 0; j < PageRank[i].size(); j++) {
                    cout << PageRank[i][j] << " ";
                }
                cout << endl;
            }
            ofstream out;
            out.open("pageRank.csv");
            for (int i = 0; i < PageRank.size(); i++)
            {
                out << i << ',';
                for (int j = 0; j < PageRank[i].size(); j++)
                {
                    out << PageRank[i][j] << ',';
                }
                out << '\n';
            }
            return;//terminate the function
        }
    }
    for (int i = 0; i < n && MIN != MAX; i++)
    {
        PageRank[i][0] = PageRank[i][1] - MIN / MAX - MIN;//normalizing
        
    }
}
vector<string> Find_Keyword(string Keyword)
{
    vector<vector<string>> web_Keywords = Get_Keywords("Keywords.csv");
    //print(web_Keywords);
    vector<string> websites;
    for (int i = 0; i < web_Keywords.size(); i++)
    {
        for (int j = 2; j < web_Keywords[i].size(); j++)
        {
            if (web_Keywords[i][j] == Keyword)
                websites.push_back(web_Keywords[i][1]);
        }
    }
    //print(websites);
    return websites;
}
vector<string> Find_Similarity(vector<string> web1 , vector<string> web2)
{
    vector<string> web3;
    for (int i = 0; i < web1.size(); i++)
    {
        for (int j = 0; j < web2.size(); j++)
        {
            if (web1[i] == web2[j])
                web3.push_back(web1[i]);
        }
    }
    return web3;
}
bool Is_Exist(vector<string> web, string key)
{
    for (int i = 0; i < web.size(); i++)
        if (web[i] == key)
            return 1;
    return 0;
}
vector<string> Merge(vector<string> web1, vector<string> web2)
{
    vector<string> web3;
    for (int i = 0; i < web1.size(); i++)
    {
        web3.push_back(web1[i]);
    }
    for (int j = 0; j < web2.size(); j++)
    {
        if(!Is_Exist(web3, web2[j]))
            web3.push_back(web2[j]);
    }
    return web3;
}
vector<string> AddOR(vector<string> keys)
{
    int k = 0;
    for (int i = 0; i < keys.size()-1; i++)
        if ((keys[i] != "AND" && keys[i] != "OR") && (keys[i+1] != "AND" && keys[i+1] != "OR"))
            keys.insert(keys.begin() + i+1, "OR");
    return keys;
}
void UpdateClicks(string page)
{
    
    vector<vector<int>> arr = read_excel_file("clicks.csv");
    vector<vector<string>> pages = Get_Keywords("Keywords.csv");
    int pageNumber;
    for (int i = 0; i < pages.size(); i++)
    {
        if (pages[i][1] == page)
        {
            pageNumber = i;
            break;
        }
    }
    arr[pageNumber][1] = arr[pageNumber][1] + 1;

    ofstream out;
    out.open("clicks.csv");
    for (int i = 0; i < arr.size(); i++)
    {
        out << i << ',' << arr[i][1] <<'\n';
    }
}
void UpdateImpressions(vector<string> pgs)
{

    vector<vector<int>> arr = read_excel_file("impressions.csv");
    vector<vector<string>> pages = Get_Keywords("Keywords.csv");
    int pageNumber;
    for (int k = 0; k < pgs.size(); k++)
    {
        for (int i = 0; i < pages.size(); i++)
        {
            if (pages[i][1] == pgs[k])
            {
                pageNumber = i;
                break;
            }
        }
        arr[pageNumber][1] = arr[pageNumber][1] + 1;
    }
    ofstream out;
    out.open("impressions.csv");
    for (int i = 0; i < arr.size(); i++)
    {
        out << i << ',' << arr[i][1] << '\n';
    }
}
vector<double> getScore()
{
    vector<vector<double>> impressions = read_excel_file_2("impressions.csv");
    vector<vector<double>> pageRank = read_excel_file_2("pageRank.csv");
    vector<vector<int>> clicks = read_excel_file("clicks.csv");
    vector<double> scores;
    for (int i = 0; i < pageRank.size(); i++)
    {
        double frac = (0.1 * impressions[i][1]) / (1.0 + 0.1 * impressions[i][1]);
        scores.push_back(0.4 * pageRank[i][2] + ((1 - frac) * pageRank[i][2] + frac * clicks[i][1]) * 0.6);
    }
    return scores;
}
vector<int> sort_by_value(vector<int> index, vector<double> values) {
    vector<pair<double, int>> temp;
    for (int i = 0; i < index.size(); i++) {
        temp.push_back({ values[index[i]], index[i] });
    }
    sort(temp.begin(), temp.end(), std::greater<pair<double, int>>());
    vector<int> result;
    for (int i = 0; i < temp.size(); i++) {
        result.push_back(temp[i].second);
    }
    return result;
}
vector<string> sort(vector<string> pages)
{
    vector<double> scores = getScore();
    cout << endl;
    vector<int> numberPages;
    vector<vector<string>> all = Get_Keywords("Keywords.csv");
    int pageNumber;
    for (int i = 0; i < all.size() - 1; i++)
    {
        for (int j = 0; j < pages.size(); j++)
        {
            if (all[i][1] == pages[j])
            {
                numberPages.push_back(i);
            }
        }

    }
    vector<int> result = sort_by_value(numberPages, scores);
    vector<string> finalResults;
    for (int j = 0; j < result.size(); j++)
    {
         finalResults.push_back(all[result[j]][1]);
    }

    return finalResults;
}
vector<string> search(string fileName, string words)
{
    vector<string> keywords = split(words,' '); // Mohamed AND Ragab OR Abbas AND "Mohamed Abbas" 
    if(keywords.size() > 2)
        keywords = AddOR(keywords);
    vector<string>webs1;
    vector<string>webs2;
    vector<string>webs3;
    int a = 0;
    //c++ OR learning AND java OR objects AND complexity
    for (int i = 0; i < keywords.size(); i++)
    {
        if (keywords[i] == "AND")
        {
            if (a == 0)
            {
                webs1 = Find_Keyword(keywords[i - 1]);
                webs2 = Find_Keyword(keywords[i + 1]);
                webs3 = Merge(webs3, Find_Similarity(webs1, webs2));
            }
            else if (a == 1)
            {
                webs1 = Find_Keyword(keywords[i + 1]);
                webs3 = Find_Similarity(webs3, webs1);
            }
            a = 1;
        }
        else if (keywords[i] == "OR")
        {
            a = 0;
        }
        //print(webs3);
    }
    for (int i = 0; i < keywords.size(); i++)
    {
        if (keywords[i] == "AND")
        {
            
            keywords.erase(keywords.begin() + i - 1, keywords.begin() + i+2);
            i = 0;
        }
        else if (keywords[i] == "OR")
        {
            keywords.erase(keywords.begin() + i);
            i = 0;
        }
    }
    for (int i = 0; i < keywords.size(); i++)
    {
        webs3 = Merge(webs3, Find_Keyword(keywords[i]));
    }
    return webs3;
}
void printPages(vector<string> pages)
{
    cout << "Search Results" << endl;
    if (pages.size() == 0)
        cout << "No results are found...." << endl;
    for (int i = 0; i < pages.size(); i++)
        cout << "    "<<i << " - " << pages[i] << endl;
}
void Search()
{
    int option;
    bool flag = 0;
    int pageNumber;
    string word="";
    cout << "Welcome!" << endl;
    cout << "What would you like to do?" << endl;
    cout << "   1- New Search" << endl;
    cout << "   2- Exit" << endl<< endl;
    cout << "Type in your chocie : ";
    cin >> option;

    if (option == 2)
        return;
    else if (option==1)
    {
        do 
        {
            if (!flag)
            {
                word = "";
                cout << "==========================================" << endl;
                cout << "Type KeyWord : ";
                cin.ignore();
                getline(cin, word, '\n');
            }
            vector<string> pages;
            pages = search("Keywords.csv", word);
            pages = sort(pages);
            UpdateImpressions(pages);
            printPages(pages);
            cout << "Would you like to :  " << endl;
            if (pages.size() != 0)
                cout << "   1- Choose a webpage to open" << endl;
            cout << "   2- New search" << endl;
            cout << "   3- Exit"<<endl;
            cout <<endl <<"Type in your chocie : ";
            cin >> option;
            if (option == 1)
            {
                cout << "Choose the page you want to open : ";
                cin >> pageNumber;
                UpdateClicks(pages[pageNumber]);
                cout << "|---------------------------------------------------------------|" << endl;
                cout <<  "           You are veiwing website " << pages[pageNumber] << "." << endl;
                cout << "|---------------------------------------------------------------|" << endl;
                cout << "Would you like to :  " << endl;
                cout << "   1- Back to search results" << endl;
                cout << "   2- New search" << endl;
                cout << "   3- Exit" << endl;
                cout << endl << "Type in your chocie : ";
                cin >> option;
                if (option == 1)
                {
                    flag = 1;
                    continue;
                }
                else if (option == 2)
                {
                    flag = 0;
                    continue;
                }
                else if (option == 3)
                    return;
                else
                {
                    cout << "You Enter a wrong option!!" << endl;
                    return;
                }

            }
            else if (option == 2)
            {
                flag = 0;
                continue;
            }
            else if (option == 3)
                return;
            else
            {
                cout << "You Enter a wrong option!!" << endl;
                return;
            }
        } while (true);
    }
    else
    {
        cout << "You Enter a wrong option!!" << endl;
        return;
    }
}

int main() 
{
    //calculatePageRank(5);
    Search();
    return 0;
}


