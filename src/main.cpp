#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// Start LoadData
class LoadData {
    private:
        int numberOfFrames;
        vector<int> pages;

    public:
        LoadData();
        LoadData(string path);

        int getNumberOfFrames();
        vector<int> getPages();

        void printData();
};

LoadData::LoadData(string pathParam) {
    ifstream myfile(pathParam);
    int index;

    if (myfile.is_open()) {

        myfile >> numberOfFrames;

        while (myfile.good()) {
            int page;

            myfile >> page;

            pages.push_back(page);

            index++;
        }
    } else cout << "Unable to open file in path" << pathParam;
}

int LoadData::getNumberOfFrames() {
    return numberOfFrames;
}

vector<int> LoadData::getPages() {
    return pages;
}

void LoadData::printData() {
    cout << "Number of Frames: " << numberOfFrames << endl;

    for (int i = 0; i < pages.size(); i++) {
        cout << "Page " << i << ": " << pages[i] << endl;
    }
}
// End LoadData

// Start FIFO
class FIFO {
    private:
        int numberOfFrames;
        vector<int> pages;
        vector<int> pageFaults;

    public:
        FIFO(LoadData data);
        void run();
        void printPageFaults();
};

FIFO::FIFO(LoadData data) {
    numberOfFrames = data.getNumberOfFrames();
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
            if (frameList.size() < numberOfFrames) {
                frameList.push_back(page);
            } else {
                frameList[indexFrame] = page;
                indexFrame++;

                if (indexFrame == numberOfFrames) {
                    indexFrame = 0;
                }
            }

            pageFaults.push_back(page);
        }
    }
}

void FIFO::printPageFaults() {
    cout << "FIFO " << pageFaults.size() << endl;
}
// End FIFO

// Sart OTM
class OTM {
    private:
        int numberOfFrames;
        vector<int> pages;
        vector<int> pageFaults;

    public:
        OTM(LoadData data);
        void run();
        void printPageFaults();
};

OTM::OTM(LoadData data) {
    numberOfFrames = data.getNumberOfFrames();
    pages = data.getPages();
}

struct Frame {
    int page;
    int index;
};

void OTM::run() {
    vector<Frame> frameList;
    int indexFrame = 0;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        bool pageFault = true;

        for (int j = 0; j < frameList.size(); j++) {
            if (page == frameList[j].page) {
                pageFault = false;
            }
        }

        if (pageFault) {
            if (frameList.size() < numberOfFrames) {
                Frame newFrame;
                newFrame.page = page;
                newFrame.index = i;

                frameList.push_back(newFrame);
            } else {
                int index = 0;
                int maxIndex = 0;
                int maxCount = 0;

                for (int j = 0; j < frameList.size(); j++) {
                    int count = 0;

                    for (int k = i + 1; k < pages.size(); k++) {
                        if (frameList[j].page == pages[k]) {
                            break;
                        }
                        count++;
                    }

                    if (count > maxCount) {
                        maxCount = count;
                        maxIndex = j;
                    }
                }

                frameList[maxIndex].page = page;
                frameList[maxIndex].index = i;
            }

            pageFaults.push_back(page);
        }
    }
}

void OTM::printPageFaults() {
    cout << "OTM " << pageFaults.size() << endl;
}
// End OTM

// Start LRU
class LRU {
    private:
        int numberOfFrames;
        vector<int> pages;
        vector<int> pageFaults;

    public:
        LRU(LoadData data);
        void run();
        void printPageFaults();
};

LRU::LRU(LoadData data) {
    numberOfFrames = data.getNumberOfFrames();
    pages = data.getPages();
}

void LRU::run() {
    vector<Frame> frames;
    int indexFrame = 0;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        bool pageFault = true;

        for (int j = 0; j < frames.size(); j++) {
            if (page == frames[j].page) {
                pageFault = false;
                frames[j].index = i;
            }
        }

        if (pageFault) {
            if (frames.size() < numberOfFrames) {
                Frame newFrame;
                newFrame.page = page;
                newFrame.index = i;

                frames.push_back(newFrame);
            } else {
                int index = 0;
                int maxIndex = 0;
                int maxCount = 0;

                for (int j = 0; j < frames.size(); j++) {
                    int count = i - frames[j].index;

                    if (count > maxCount) {
                        maxCount = count;
                        maxIndex = j;
                    }
                }

                frames[maxIndex].page = page;
                frames[maxIndex].index = i;
            }

            pageFaults.push_back(page);
        }
    }
}

void LRU::printPageFaults() {
    cout << "LRU " << pageFaults.size() << endl;
}
// End LRU

int main (void) {
    LoadData data("./instances/instance-1.txt");

    FIFO fifo(data);
    fifo.run();
    fifo.printPageFaults();

    OTM otm(data);
    otm.run();
    otm.printPageFaults();

    LRU lru(data);
    lru.run();
    lru.printPageFaults();

    return 0;
}