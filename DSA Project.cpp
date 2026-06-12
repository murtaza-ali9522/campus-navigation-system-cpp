#include <iostream>
#include <string>
#include <queue>
using namespace std;

class AdjNode {
public:
    string dest;
    string direction;
    AdjNode* next;
    AdjNode* prev;

    AdjNode(string d, string dir) {
        dest = d;
        direction = dir;
        next = NULL;
        prev = NULL;
    }
};

class AdjList {
public:
    AdjNode* head;

    AdjList() {
        head = NULL;
    }

    ~AdjList() {
        AdjNode* current = head;
        while (current != NULL) {
            AdjNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void addNode(string dest, string direction) {
        AdjNode* node = new AdjNode(dest, direction);
        node->next = head;
        if (head != NULL)
            head->prev = node;
        head = node;
    }

    bool removeNode(string dest) {
        AdjNode* ptr = head;
        while (ptr != NULL) {
            if (ptr->dest == dest) {
                if (ptr->prev) ptr->prev->next = ptr->next;
                else head = ptr->next;
                if (ptr->next) ptr->next->prev = ptr->prev;
                delete ptr;
                return true;
            }
            ptr = ptr->next;
        }
        return false;
    }
};

class Graph {
private:
    int numVertices;
    string* vertices;
    AdjList* array;

    int getVertexIndex(string name) {
        for (int i = 0; i < numVertices; i++)
            if (vertices[i] == name)
                return i;
        return -1;
    }

public:
    Graph(int n, string verts[]) {
        numVertices = n;
        vertices = new string[numVertices];
        array = new AdjList[numVertices];

        for (int i = 0; i < numVertices; i++)
            vertices[i] = verts[i];
    }

    ~Graph() {
        delete[] vertices;
        delete[] array;
    }

    void addEdge(string src, string dest, string direction) {
        int index = getVertexIndex(src);
        if (index != -1)
            array[index].addNode(dest, direction);
    }

    void addBiEdge(string src, string dest, string dir1, string dir2) {
        addEdge(src, dest, dir1);
        addEdge(dest, src, dir2);
    }

    void showAllBlocks() {
        cout << "\nCampus Blocks:\n";
        for (int i = 0; i < numVertices; i++)
            cout << "- " << vertices[i] << endl;
    }

    bool isValidBlock(string name) {
        return getVertexIndex(name) != -1;
    }

    // BFS Reachable Locations
    void BFS(string start) {
        int startIndex = getVertexIndex(start);
        if (startIndex == -1) {
            cout << "Invalid location!\n";
            return;
        }

        bool* visited = new bool[numVertices]{false};
        queue<int> q;

        visited[startIndex] = true;
        q.push(startIndex);

        cout << "\nReachable locations from " << start << ":\n";

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            cout << vertices[current] << endl;

            AdjNode* ptr = array[current].head;
            while (ptr != NULL) {
                int idx = getVertexIndex(ptr->dest);
                if (idx != -1 && !visited[idx]) {
                    visited[idx] = true;
                    q.push(idx);
                }
                ptr = ptr->next;
            }
        }
        delete[] visited;
    }

    // BFS Shortest Path
    void shortestPath(string start, string end) {
        int startIndex = getVertexIndex(start);
        int endIndex = getVertexIndex(end);

        if (startIndex == -1 || endIndex == -1) {
            cout << "Invalid start or destination!\n";
            return;
        }

        bool* visited = new bool[numVertices]{false};
        int* parent = new int[numVertices];
        string* directionTaken = new string[numVertices];

        for (int i = 0; i < numVertices; i++) {
            parent[i] = -1;
            directionTaken[i] = "";
        }

        queue<int> q;
        visited[startIndex] = true;
        q.push(startIndex);

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            AdjNode* ptr = array[current].head;
            while (ptr != NULL) {
                int idx = getVertexIndex(ptr->dest);
                if (idx != -1 && !visited[idx]) {
                    visited[idx] = true;
                    parent[idx] = current;
                    directionTaken[idx] = ptr->direction;
                    q.push(idx);
                }
                ptr = ptr->next;
            }
        }

        if (!visited[endIndex]) {
            cout << "No path exists.\n";
            delete[] visited;
            delete[] parent;
            delete[] directionTaken;
            return;
        }

        int* path = new int[numVertices];
        int count = 0;
        for (int v = endIndex; v != -1; v = parent[v])
            path[count++] = v;

        cout << "\nShortest Path with Directions:\n";
        for (int i = count - 1; i > 0; i--) {
            int from = path[i];
            int to = path[i - 1];
            cout << vertices[from] << " --(" << directionTaken[to] << ")--> ";
        }
        cout << vertices[path[0]] << endl;

        delete[] visited;
        delete[] parent;
        delete[] directionTaken;
        delete[] path;
    }
};

void displayMenu() {
    cout << "\n===== Campus Navigation =====\n";
    cout << "1. Show all blocks\n";
    cout << "2. Show reachable locations\n";
    cout << "3. Find shortest path\n";
    cout << "4. Exit\n";
    cout << "Enter choice: ";
}

int main() {
    string blocks[] = {"Entrance", "LTC", "IAAA", 
                       "FMC", "NCSA", "Parking", "A Block", 
                       "B Block", "C Block", "Admin Block", "FNC"};
    int n = sizeof(blocks) / sizeof(blocks[0]);
    Graph graph(n, blocks);
 
    // Entrance
    graph.addBiEdge("Entrance", "Parking", "turn Right then walk downward", "Walk straight upward slope then turn left ");
    graph.addBiEdge("Entrance", "LTC", "Turn Right Walk (10m)", "Walk straight then turn Left Walk (10m)");
    graph.addBiEdge("Entrance", "IAAA", "Straight walk (20m) then turn Right", "Walk straight then turn Left Walk (20m)");
    graph.addBiEdge("Entrance", "FMC", "Straight walk (25m) then turn Right", "Walk straight then turn Left Walk (25m)");
    graph.addBiEdge("Entrance", "B Block", "Straight walk (30m) then turn Right", "Walk straight then turn Left Walk (30m)");
    graph.addBiEdge("Entrance", "Admin Block", "Straight walk (50m) then turn Right", "Walk straight then turn Left Walk (50m)");
    graph.addBiEdge("Entrance", "FNC", "Straight walk (75m) then turn Right", "Walk straight then turn Left Walk (75m)");
    graph.addBiEdge("Entrance", "C Block", "Straight walk (70m) then turn Right walk a little then turn left and walk ",
                    "Walk straight then turn right Walk then turn left and walk (70m)");

    // LTC & IAAA
    graph.addBiEdge("LTC", "IAAA", "Straight then turn right walk (15m) turn right",
                    "Straight then turn left walk (15m) then turn left");
    graph.addBiEdge("IAAA", "FMC", "walk Left turn Right walk straight",
                    "straight turn left walk and then turn left");

    // FMC
    graph.addBiEdge("FMC", "NCSA", " walk Left", "walk Right");
    graph.addBiEdge("FMC", "Parking", "walk Right", "walk Left");
    graph.addBiEdge("FMC", "B Block", "walk Left", "walk Right");

    // Admin & Blocks
    graph.addBiEdge("Admin Block", "B Block",
                    "walk straight then turn left walk (20m) turn left",
                    "walk straight then turn right walk (20m) turn right");
    graph.addBiEdge("B Block", "A Block", "Left", "Right");
    graph.addBiEdge("B Block", "NCSA", "Right", "Left");
    graph.addBiEdge("C Block", "Admin Block", "walk straight",
                    "walk straight then turn right walk (20m) turn right");

    int choice;
    string start, end;
    while (true) {
        displayMenu();
        if (!(cin >> choice)) {
            cout << "Invalid input!\n";
            break;
        }

        switch (choice) {
        case 1:
            graph.showAllBlocks();
            break;

        case 2:
            cout << "Enter starting block: ";
            cin.ignore();
            getline(cin, start);
            graph.BFS(start);
            break;

        case 3:
            cout << "Enter starting block: ";
            cin.ignore();
            getline(cin, start);
            cout << "Enter destination block: ";
            getline(cin, end);
            graph.shortestPath(start, end);
            break;

        case 4:
            cout << "Exiting...\n";
            return 0;

        default:
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}

