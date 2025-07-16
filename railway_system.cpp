#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to, capacity, rev;
};

class RailwaySystem {
    int V;
    vector<vector<pair<int,int>>> adj;
    vector<vector<Edge>> flowGraph;
    vector<int> platformAssigned;
    int platformCount;
    queue<int> bookingQueue;

public:
    vector<string> stationNames;
    unordered_map<string, int> stationIndex;
    vector<string> indexToStation;

    RailwaySystem(int V, int platformCount, vector<string> stationNames) : V(V), platformCount(platformCount), stationNames(stationNames) {
        adj.resize(V);
        flowGraph.resize(V);
        platformAssigned.assign(V, -1);
        indexToStation = stationNames;
        for (int i = 0; i < V; ++i) {
            stationIndex[stationNames[i]] = i;
        }
    }

    void addEdge(int u, int v, int w, int capacity) {
        adj[u].push_back({v,w});
        adj[v].push_back({u,w});

        flowGraph[u].push_back({v, capacity, (int)flowGraph[v].size()});
        flowGraph[v].push_back({u, 0, (int)flowGraph[u].size()-1});
    }

    int heuristic(int u, int dest) {
        return 0; 
    }

    int aStar(int src, int dest, vector<int>& path) {
        vector<int> dist(V, INT_MAX), prev(V, -1);
        dist[src] = 0;

        using T = pair<int, int>;
        priority_queue<T, vector<T>, greater<T>> pq;
        pq.push({heuristic(src, dest), src});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            if (u == dest) break;

            for (auto &p : adj[u]) {
                int v = p.first, w = p.second;
                int ndist = dist[u] + w;
                if (ndist < dist[v]) {
                    dist[v] = ndist;
                    prev[v] = u;
                    pq.push({ndist + heuristic(v, dest), v});
                }
            }
        }

        if (dist[dest] == INT_MAX) return -1;

        path.clear();
        for (int at = dest; at != -1; at = prev[at])
            path.push_back(at);
        reverse(path.begin(), path.end());
        return dist[dest];
    }

    bool bfs(int s, int t, vector<int> &parent) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(s);
        visited[s] = true;
        parent.assign(V, -1);

        while(!q.empty()) {
            int u = q.front(); q.pop();
            for (int i=0; i<(int)flowGraph[u].size(); i++) {
                Edge &e = flowGraph[u][i];
                if (!visited[e.to] && e.capacity > 0) {
                    visited[e.to] = true;
                    parent[e.to] = u;
                    q.push(e.to);
                    if (e.to == t) return true;
                }
            }
        }
        return false;
    }

    int fordFulkerson(int s, int t) {
        int maxFlow = 0;
        vector<int> parent(V);
        while(bfs(s, t, parent)) {
            int pathFlow = INT_MAX;
            for (int v=t; v!=s; v=parent[v]) {
                int u = parent[v];
                for (Edge &e : flowGraph[u]) {
                    if (e.to == v && e.capacity > 0) {
                        pathFlow = min(pathFlow, e.capacity);
                        break;
                    }
                }
            }
            for (int v=t; v!=s; v=parent[v]) {
                int u = parent[v];
                for (int i=0; i<(int)flowGraph[u].size(); i++) {
                    if (flowGraph[u][i].to == v) {
                        flowGraph[u][i].capacity -= pathFlow;
                        flowGraph[v][flowGraph[u][i].rev].capacity += pathFlow;
                        break;
                    }
                }
            }
            maxFlow += pathFlow;
        }
        return maxFlow;
    }

    bool assignPlatform(int node, int trainId) {
        if (platformAssigned[node] == -1) {
            platformAssigned[node] = trainId;
            return true;
        }
        return false;
    }

    bool releasePlatform(int node) {
        if (platformAssigned[node] != -1) {
            platformAssigned[node] = -1;
            return true;
        }
        return false;
    }

    void printPlatforms() {
        cout << "Platform Assignments:\n";
        for (int i=0; i<V; i++) {
            cout << stationNames[i] << ": ";
            if (platformAssigned[i] == -1) cout << "Free\n";
            else cout << "Train " << platformAssigned[i] << "\n";
        }
    }

    void bookTicket(int trainId) {
        bookingQueue.push(trainId);
        cout << "Ticket booked for Train " << trainId << "\n";
    }

    void cancelTicket() {
        if (bookingQueue.empty()) {
            cout << "No tickets to cancel.\n";
            return;
        }
        int trainId = bookingQueue.front();
        bookingQueue.pop();
        cout << "Ticket cancelled for Train " << trainId << "\n";
    }

    void showBookings() {
        cout << "Current Booking Queue: ";
        queue<int> temp = bookingQueue;
        if (temp.empty()) cout << "Empty\n";
        else {
            while (!temp.empty()) {
                cout << temp.front() << " ";
                temp.pop();
            }
            cout << "\n";
        }
    }
};

int main() {
    int V = 6;
    int platformCount = 3;
    vector<string> stationNames = {"Dehradun", "Haridwar", "Rishikesh", "Saharanpur", "Roorkee", "Delhi"};
    RailwaySystem rs(V, platformCount, stationNames);

    rs.addEdge(0, 1, 53, 10);
    rs.addEdge(0, 2, 45, 8);
    rs.addEdge(1, 4, 32, 15);
    rs.addEdge(2, 1, 25, 7);
    rs.addEdge(1, 3, 65, 10);
    rs.addEdge(3, 4, 55, 9);
    rs.addEdge(4, 5, 170, 20);
    rs.addEdge(3, 5, 190, 18);

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Find shortest path (A*)\n";
        cout << "2. Find max flow (Ford-Fulkerson)\n";
        cout << "3. Assign platform\n";
        cout << "4. Release platform\n";
        cout << "5. Show platform assignments\n";
        cout << "6. Book ticket\n";
        cout << "7. Cancel ticket\n";
        cout << "8. Show bookings\n";
        cout << "9. Exit\n";
        cout << "Choice: ";
        int choice; cin >> choice;

        if (choice == 1) {
            string sName, dName;
            cout << "Enter source city: ";
            cin >> ws; getline(cin, sName);
            cout << "Enter destination city: ";
            getline(cin, dName);

            if (rs.stationIndex.count(sName) == 0 || rs.stationIndex.count(dName) == 0) {
                cout << "Invalid city name.\n";
                continue;
            }

            int s = rs.stationIndex[sName], d = rs.stationIndex[dName];
            vector<int> path;
            int dist = rs.aStar(s, d, path);
            if (dist == -1) cout << "No path found\n";
            else {
                cout << "Shortest distance: " << dist << "\nPath: ";
                for (int i = 0; i < path.size(); ++i) {
                    cout << rs.indexToStation[path[i]];
                    if (i != path.size()-1) cout << " -> ";
                }
                cout << "\n";
            }
        }
        else if (choice == 2) {
            int s,t;
            cout << "Enter source and sink index for max flow: ";
            cin >> s >> t;
            int flow = rs.fordFulkerson(s,t);
            cout << "Max flow: " << flow << "\n";
        }
        else if (choice == 3) {
            int station, trainId;
            cout << "Enter station index and train id to assign platform: ";
            cin >> station >> trainId;
            if (station<0 || station>=V) { cout << "Invalid station\n"; continue;}
            if (rs.assignPlatform(station, trainId)) cout << "Platform assigned\n";
            else cout << "Platform already occupied\n";
        }
        else if (choice == 4) {
            int station;
            cout << "Enter station index to release platform: ";
            cin >> station;
            if (station<0 || station>=V) { cout << "Invalid station\n"; continue;}
            if (rs.releasePlatform(station)) cout << "Platform released\n";
            else cout << "No platform assigned at this station\n";
        }
        else if (choice == 5) {
            rs.printPlatforms();
        }
        else if (choice == 6) {
            int trainId;
            cout << "Enter train id to book ticket: ";
            cin >> trainId;
            rs.bookTicket(trainId);
        }
        else if (choice == 7) {
            rs.cancelTicket();
        }
        else if (choice == 8) {
            rs.showBookings();
        }
        else if (choice == 9) {
            cout << "Exiting...\n";
            break;
        }
        else {
            cout << "Invalid choice\n";
        }
    }

    return 0;
}
