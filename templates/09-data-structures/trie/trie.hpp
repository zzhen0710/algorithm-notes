#pragma once

#include <array>
#include <string>
#include <vector>

// 连续字符表：Trie<>是a..z，Trie<2,'0'>是0/1；查询区分缺边与完整终点。
template<int Sigma = 26, char Base = 'a'>
struct Trie {
    static_assert(Sigma > 0, "alphabet must be nonempty");

    struct Node {
        std::array<int, Sigma> next;
        int pass = 0, end = 0;
        Node() { next.fill(-1); }
    };
    std::vector<Node> tr;

    Trie() : tr(1) {}
    void clear() { tr.assign(1, Node{}); }

    void insert(const std::string& s) {
        int u = 0;
        ++tr[u].pass;
        for (char ch : s) {
            int c = ch - Base; // 前提：0<=c<Sigma。
            if (tr[u].next[c] == -1) {
                int v = static_cast<int>(tr.size());
                tr[u].next[c] = v;
                tr.emplace_back(); // 不保留跨扩容的节点引用。
            }
            u = tr[u].next[c];
            ++tr[u].pass;
        }
        ++tr[u].end;
    }

    int find_node(const std::string& s) const {
        int u = 0;
        for (char ch : s) {
            u = tr[u].next[ch - Base];
            if (u == -1) return -1;
        }
        return u;
    }

    int count(const std::string& s) const {
        int u = find_node(s);
        return u == -1 ? 0 : tr[u].end;
    }

    int prefix_count(const std::string& s) const {
        int u = find_node(s);
        return u == -1 ? 0 : tr[u].pass;
    }

    // 任意一方是另一方的前缀即可；重复插入重复计数，相等只算一次。
    int prefix_related_count(const std::string& s) const {
        int u = 0, ans = 0;
        for (char ch : s) {
            ans += tr[u].end; // 当前节点仍是 s 的严格前缀。
            u = tr[u].next[ch - Base];
            if (u == -1) return ans;
        }
        return ans + tr[u].pass; // 等长与更长；终点 end 已在 pass 内。
    }
};
