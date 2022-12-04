#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// Start LoadData
class LoadData {
    private:
        int frames;
        vector<int> pages;

    public:
        LoadData();
        LoadData(string path);

        int getFrames();
        vector<int> getPages();

        void printData();
};

LoadData::LoadData(string pathParam) {
    ifstream myfile(pathParam);
    int index;

    if (myfile.is_open()) {

        myfile >> frames;

        while (myfile.good()) {
            int page;

            myfile >> page;

            pages.push_back(page);

            index++;
        }
    } else cout << "Unable to open file in path" << pathParam;
}

int LoadData::getFrames() {
    return frames;
}

vector<int> LoadData::getPages() {
    return pages;
}

void LoadData::printData() {
    cout << "Frames: " << frames << endl;

    for (int i = 0; i < pages.size(); i++) {
        cout << "Page " << i << ": " << pages[i] << endl;
    }
}
// End LoadData

// Start FIFO
class FIFO {
    private:
        int frames;
        vector<int> pages;
        vector<int> pageFaults;

    public:
        FIFO(LoadData data);
        void run();
        void printPageFaults();
};

FIFO::FIFO(LoadData data) {
    frames = data.getFrames();
    pages = data.getPages();
}

void FIFO::run() {
    vector<int> frameList;
    int indexFrame = 0;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        bool pageFault = true;

        for (int j = 0; j < frameList.size(); j++) {
            if (page == frameList[j]) {
                pageFault = false;
            }
        }

        if (pageFault) {
            if (frameList.size() < frames) {
                frameList.push_back(page);
            } else {
                frameList[indexFrame] = page;
                indexFrame++;

                if (indexFrame == frames) {
                    indexFrame = 0;
                }
            }

            pageFaults.push_back(page);
        }
    }
}

void FIFO::printPageFaults() {
    cout << "Page Faults: " << pageFaults.size() << endl;

    for (int i = 0; i < pageFaults.size(); i++) {
        cout << "Page Fault " << i << ": " << pageFaults[i] << endl;
    }
}
// End FIFO


int main (void) {
    LoadData loadData("./instances/instance-1.txt");

    FIFO fifo(loadData);
    fifo.run();
    fifo.printPageFaults();

    return 0;
}