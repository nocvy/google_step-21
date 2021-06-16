#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

/* ----- キーワードのページIDを取得する ----- */
string getID(map<string, string> pages, string title){
    bool sFinish = false, gFinish = false;
    string startID, goalID;

    for (const auto& page : pages) {
        if (page.second == title) {
            startID = page.first;
            break;
        }
    }
    return startID;
}

/* ----- 経路を取得する ----- */
vector<pair<string, string> > getPATH(vector<pair<string, string> > candidate, map<string, string> pages, string goalID){
    vector<pair<string, string> > path;
    reverse(candidate.begin(), candidate.end()); // 逆順に探した方が効率がいいのでreverseする

    string candidateID = goalID;
    while (candidateID != "-1") {
        for (pair<string, string> c : candidate) {
            if (c.first == candidateID) {
                path.push_back(make_pair(pages[c.first], c.first));
                candidateID = c.second;
            }
        }
    }
    reverse(path.begin(), path.end());
    return path;
}

/* ----- 幅優先探索(BFS) & 経路の表示 ----- */
void bfs(map<string, string> pages, map<string, set<string> > links, string start, string goal){
    string startID = getID(pages, start);
    string goalID = getID(pages, goal);

    queue<pair<string, string> > que;         // (現在のID、直前のID)
    vector<pair<string, string> > candidate;  // 後でパスを辿るため履歴を保存
    vector<pair<string, string> > path;       // 最終的な経路を保存
    vector<string> looked;                    // 既に探索済みのIDを保存
    bool ans = false;                         // startからgoalに辿り着けるか
    que.push(make_pair(startID, "-1"));

    while (!que.empty()) {
        pair<string, string> v = que.front();
        que.pop();

        if (v.first == goalID) {
            candidate.push_back(v);
            ans = true;
            break;
        }
        else {
            candidate.push_back(v);
            looked.push_back(v.first);
            for (string x : links[v.first]) {
                if (find(looked.begin(),looked.end(),x) == looked.end())
                    que.push(make_pair(x,v.first));
            }
        }
    }

    if (ans) { /* --- 経路が見つかったとき --- */
        path = getPATH(candidate, pages, goalID);

        cout << "Shows the route from \"" << start << "\" to \"" << goal << "\"." << endl;
        for (pair<string, string> p: path) {
            cout << p.first << " (pageID : " << p.second << ")" << endl;
        }
    }
    else { /* --- 経路が見つからなかったとき --- */
        cout << "You can't go from \"" << start << "\" to \"" << goal << "\"." << endl;
    }

}

int main() {
    map<string, string> pages;
    map<string, set<string> > links;
    string start, goal;

    {	/* --- ページ情報の読み込み --- */
        ifstream file("data/pages_small.txt");
        string data;
        while (getline(file, data)) {
            auto index = data.find('\t');
            auto id = data.substr(0, index);
            auto title = data.substr(index + 1, data.size() - id.size() - 1);
            pages[id] = title;
        }
    }

    {	/* --- リンク情報の読み込み --- */
        ifstream file("data/links_small.txt");
        string data;
        while (getline(file, data)) {
            auto index = data.find('\t');
            auto from = data.substr(0, index);
            auto to = data.substr(index + 1, data.size() - from.size() - 1); 
            links[from].insert(to);
        }
    }

    /* 探索したいキーワードの入力 */
    cout << "Enter start page : ";
    cin >> start;
    cout << "Enter goal page : ";
    cin >> goal;
    cout << endl;

    bfs (pages, links, start, goal);

    return 0;
}
