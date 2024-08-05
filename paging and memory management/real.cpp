#include <bits/stdc++.h>
#include <fstream>

using namespace std;
ofstream fileout;
// class MainMemoroy{

// };

class Process
{
public:
    int pid;
    int size;
    vector<int> pageTable;
    int start;
    int end;
    vector<string> info;
    int kill;
    string name;
    int P;

    Process(int pid, int P) : pid(pid), P(P), kill(1) {}

    vector<int> &getPageTable()
    {
        return pageTable;
    }

    void load(string s)
    {
        // cout<<s<<endl;
        cout << "loading " << s << endl;
        name = s;
        std::ifstream file(s);

        // Check if the file was opened successfully
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                info.push_back(line);
                // std::cout << line << std::endl; // Do something with the line
            }
            size = (stoi(info[0]) * 1024) / P;
        }
        else
        {
            std::cerr << "Unable to open file: " << s << std::endl;
            fileout << "Unable to open file: " << s << endl;
            size = -1;
        }

        // cout<<size<<endl;
    }
};

class Commando
{
public:
    vector<int> &MainMemory;
    vector<int> &VirtualMemory;
    vector<int> &mm;
    vector<int> &vm;
    vector<Process> procs;
    int nextpid;
    vector<int> mprocs, vprocs;
    deque<int> dq;
    int P;

    Commando(vector<int> &m, vector<int> &v, vector<int> &mm, vector<int> &vm, int P) : MainMemory(m), VirtualMemory(v), nextpid(1), mm(mm), vm(vm), P(P) {}

    void load(string s)
    {

        string p = "";
        int done;
        // cout<<s<<endl;
        for (char c : s)
        {
            if (c == ' ')
            {
                // cout<<"lll"<<p<<endl;
                procs.push_back(Process(nextpid, P));
                procs.back().load(p);
                procs.back().pid = nextpid;
                string ily = p;
                p = "";

                done = 0;
                int sz = procs.back().size;
                if (sz == -1)
                {
                    fileout << "Ivalid file " << ily << endl;
                }
                else
                {
                    int res = 0;
                    for (int i = 0; i < MainMemory.size(); i++)
                    {
                        if (MainMemory[i] == -1)
                        {
                            res++;
                            if (res == sz)
                            {
                                done = 1;
                                break;
                            }
                        }
                    }
                    if (done)
                    {

                        for (int i = 0; i < MainMemory.size(); i++)
                        {
                            if (MainMemory[i] == -1)
                            {
                                MainMemory[i] = nextpid;
                                procs[nextpid - 1].pageTable.push_back(i);
                                res--;
                                if (res == 0)
                                {
                                    break;
                                }
                            }
                        }
                        mprocs.push_back(nextpid);
                        nextpid++;
                    }
                    else
                    {

                        res = 0;
                        for (int i = 0; i < VirtualMemory.size(); i++)
                        {
                            if (VirtualMemory[i] == -1)
                            {
                                res++;
                                if (res == sz)
                                {
                                    done = 1;
                                    break;
                                }
                            }
                        }
                        if (done)
                        {
                            for (int i = 0; i < VirtualMemory.size(); i++)
                            {
                                if (VirtualMemory[i] == -1)
                                {
                                    VirtualMemory[i] = nextpid;
                                    procs[nextpid - 1].pageTable.push_back(i);
                                    res--;
                                    if (res == 0)
                                    {
                                        break;
                                    }
                                }
                            }
                            vprocs.push_back(nextpid);
                            nextpid++;
                        }
                        else
                        {
                            fileout << "cannot load " << ily << endl;
                            fileout << "ignore successfull message " << endl;
                        }
                    }
                }
                // cout<<sz<<endl;
            }
            else
            {
                if (c != ' ')
                {
                    p += c;
                }
            }
        }
        // cout<<procs[nextpid-2].size<<endl;

        //  procs[nextpid-1].load(p);

        //         // int done =0;
        //         done=0;

        //         nextpid++;
    }

    void run(int pp)
    {
        if (procs[pp - 1].kill == -1)
        {
            cout << "already killed" << endl;
            fileout << pp << " already killed ,ignore success message" << endl;
        }
        else
        {
            auto it = find(mprocs.begin(), mprocs.end(), pp);
            if (it != mprocs.end() || (it == mprocs.end() && swapin(pp) != -1))
            {
                cout << "running " << pp << endl;
                dq.push_front(pp);
                vector<string> v = procs[pp - 1].info;
                for (int i = 0; i < v.size(); i++)
                {
                    string kk = "";
                    int gs = 0;
                    string ins = "";
                    int ai = 0;
                    int a1, a2, a3;
                    for (auto c : v[i])
                    {
                        if (c == ' ')
                        {
                            // cout<<kk<<endl;
                            if (!gs)
                            {
                                ins = kk;
                                gs = 1;
                                kk = "";
                            }
                            else
                            {
                                if (ins == "add" || ins == "sub")
                                {
                                    if (ai == 0)
                                    {
                                        a1 = stoi(kk);
                                        kk = "";
                                        ai++;
                                    }
                                    else if (ai == 1)
                                    {
                                        a2 = stoi(kk);
                                        kk = "";
                                        ai++;
                                    }
                                }
                                else if (ins == "load")
                                {
                                    a1 = stoi(kk);
                                    kk = "";
                                }
                            }
                        }
                        else
                        {
                            if (c != ' ' && c != ',')
                            {
                                kk += c;
                            }
                            // cout<<kk<<endl;
                        }
                    }
                    // cout<<ins<<endl;
                    if (ins == "load")
                    {
                        a2 = stoi(kk);
                        int vpage = a2 / P;
                        int offs = a2 % P;
                        vector<int> pt = procs[pp - 1].pageTable;
                        int tp = pt[vpage] * P + offs;
                        // cout<<tp<<endl;
                        mm[tp] = a1;
                    }
                    else if (ins == "add" || ins == "sub")
                    {
                        a3 = stoi(kk);
                        int vpage = a1 / P;
                        int offs = a1 % P;
                        vector<int> pt = procs[pp - 1].pageTable;
                        int tp = pt[vpage] * P + offs;
                        if (mm[tp] != -9789)
                        {
                            int op1 = mm[tp];
                            vpage = a2 / P;
                            offs = a2 % P;
                            tp = pt[vpage] * P + offs;
                            if (mm[tp] != -9789)
                            {
                                int op2 = mm[tp];
                                vpage = a3 / P;
                                offs = a3 % P;
                                tp = pt[vpage] * P + offs;
                                if (ins == "add")
                                {
                                    mm[tp] = op1 + op2;
                                }
                                else
                                {
                                    mm[tp] = op1 - op2;
                                }
                            }
                            else
                            {
                                cout << "error" << endl;
                                fileout << "inerror op2" << endl;
                                // error();
                            }
                        }
                        else
                        {
                            cout << "error" << endl;
                            fileout << "inerror op1" << endl;
                            // error();
                        }
                    }
                    else if (ins == "print")
                    {
                        a1 = stoi(kk);
                        int vpage = a1 / P;
                        int offs = a1 % P;
                        vector<int> pt = procs[pp - 1].pageTable;
                        if (vpage >= pt.size())
                        {
                            cout << "error" << endl;
                            fileout << "inerror print" << endl;
                        }
                        else
                        {
                            int tp = pt[vpage] * P + offs;
                            if (mm[tp] != -9789)
                            {
                                cout << mm[tp] << endl;
                            }
                            else
                            {
                                cout << "error" << endl;
                                fileout << "inerror print" << endl;
                            }
                        }
                    }
                }
            }
            else
            {
                fileout << "cannot run " << pp << " ignore success message" << endl;
            }
        }
    }
    void kill(int pid)
    {
        procs[pid - 1].kill = -1;

        if (find(mprocs.begin(), mprocs.end(), pid) != mprocs.end())
        {
            for (int i = 0; i < procs[pid - 1].pageTable.size(); i++)
            {
                MainMemory[procs[pid - 1].pageTable[i]] = -1;
                for (int j = 1; j <= P; j++)
                {
                    mm[procs[pid - 1].pageTable[i] * P + j] = -9789;
                }
            }
            auto it = find(mprocs.begin(), mprocs.end(), pid);
            mprocs.erase(it);
        }
        else if (find(vprocs.begin(), vprocs.end(), pid) != vprocs.end())
        {
            for (int i = 0; i < procs[pid - 1].pageTable.size(); i++)
            {
                VirtualMemory[procs[pid - 1].pageTable[i]] = -1;
            }
            auto it = find(vprocs.begin(), vprocs.end(), pid);
            vprocs.erase(it);
        }
    }
    void listpr()
    {
        // cout << "hi" << endl;
        fileout << "listpr starts " << endl
                << endl;
        map<int, string> m;
        for (int i = 0; i < mprocs.size(); i++)
        {
            m[mprocs[i]] = procs[mprocs[i] - 1].name;
        }

        for (auto it : m)
        {
            cout << it.first << " " << it.second << endl;
            fileout << it.first << " " << it.second << endl;
        }
        map<int, string> v;
        for (int i = 0; i < vprocs.size(); i++)
        {
            v[vprocs[i]] = procs[vprocs[i] - 1].name;
        }
        for (auto it : v)
        {
            cout << it.first << " " << it.second << endl;
            fileout << it.first << " " << it.second << endl;
        }
    }
    void pte(int pid, string fileout)
    {
        vector<int> v = procs[pid - 1].pageTable;

        fstream file2;
        time_t tim = time(0);
        char *dat = ctime(&tim);
        auto it1 = find(mprocs.begin(), mprocs.end(), pid);
        auto it2 = find(vprocs.begin(), vprocs.end(), pid);
        file2.open(fileout, ios::app);
        if (file2.is_open())
        {
            if (it1 != mprocs.end())
            {
                file2 << "Main Memory" << endl;
            }
            else if (it2 != vprocs.end())
            {
                file2 << "Virtual Memory" << endl;
            }
            else
            {
                file2 << "invalid pid" << endl;
                return;
            }
            file2 << "PID: " << pid << endl;
            for (int i = 0; i < v.size(); i++)
            {
                file2 << "pte logical:" << i << " physical: " << v[i] << " date and time " << dat << endl;
            }
        }

        // cout<<"pte"<<fileout<<endl;
    }
    void pteall(string fileout)
    {
        // cout<<"pteall"<<fileout<<endl;
        map<int, string> m;
        for (int i = 0; i < mprocs.size(); i++)
        {
            m[mprocs[i]] = procs[mprocs[i] - 1].name;
        }
        for (int i = 0; i < vprocs.size(); i++)
        {
            m[vprocs[i]] = procs[vprocs[i] - 1].name;
        }
        for (auto it : m)
        {
            pte(it.first, fileout);
        }
    }
    void print(int memloc, int len)
    {
        for (int i = memloc; i < memloc + len; i++)
        {
            cout << "value at " << i << "is " << mm[i] << endl;
        }
    }

    int swapout(int pid)
    {
        if (pid > procs.size())
        {
            fileout << "pid doesnot exist ,ignore success" << endl;
            return -1;
        }
        auto it = find(mprocs.begin(), mprocs.end(), pid);

        if (it != mprocs.end())
        {

            vector<int> v = procs[pid - 1].pageTable;

            int sz = procs[pid - 1].size;
            int res = 0;
            int done = 0;
            for (int i = 0; i < VirtualMemory.size(); i++)
            {
                if (VirtualMemory[i] == -1)
                {
                    res++;
                    if (res == sz)
                    {
                        done = 1;
                        break;
                    }
                }
            }
            if (done)
            {
                int jj = 0;
                for (int i = 0; i < VirtualMemory.size(); i++)
                {
                    if (VirtualMemory[i] == -1)
                    {
                        VirtualMemory[i] = pid;
                        procs[pid - 1].pageTable[jj] = i;
                        jj++;
                        res--;
                        if (res == 0)
                        {
                            break;
                        }
                    }
                }
                for (int i = 0; i < v.size(); i++)
                {
                    MainMemory[v[i]] = -1;
                    for (int j = 1; j <= P; j++)
                    {
                        mm[v[i] * P + j] = -9789;
                    }
                    // procs[pid - 1].pageTable[i] = -1;
                }
                mprocs.erase(it);
                vprocs.push_back(pid);
                return 1;
            }
            else
            {
                // error();
                fileout << pid << " swapout not successfull, ignore success" << endl;
                return -1;
            }
        }
        fileout << pid << " swapout not successfull, ignore success" << endl;
        return -1;
    }

    int swapin(int pid)
    {
        if (pid > procs.size())
        {
            fileout << "pid doesnot exist ,ignore success" << endl;
            return -1;
        }
        auto it = find(vprocs.begin(), vprocs.end(), pid);

        vector<int> v = procs[pid - 1].pageTable;

        int sz = procs[pid - 1].size;
        int res = 0;
        int done = 0;
        for (int i = 0; i < MainMemory.size(); i++)
        {
            if (MainMemory[i] == -1)
            {
                res++;
                if (res == sz)
                {
                    done = 1;
                    break;
                }
            }
        }
        // cout << res << " res" << endl;
        if (done)
        {
            int jj = 0;
            for (int i = 0; i < MainMemory.size(); i++)
            {
                if (MainMemory[i] == -1)
                {
                    MainMemory[i] = pid;
                    procs[pid - 1].pageTable[jj] = i;
                    jj++;
                    res--;
                    if (res == 0)
                    {
                        break;
                    }
                }
            }
            for (int i = 0; i < v.size(); i++)
            {
                if (v[i] != -1)
                {
                    VirtualMemory[v[i]] = -1;
                    // procs[pid - 1].pageTable[i] = -1;
                }
            }
            vprocs.erase(it);
            mprocs.push_back(pid);
            return 1;
        }
        else
        {
            int ii;
            if (!dq.empty())
            {
                ii = dq.back();
                dq.pop_back();
            }
            else
            {
                ii = mprocs.back();
            }
            if (swapout(ii) == -1)
            {
                fileout << pid << " swapin not successfull, ignore success" << endl;
                return -1;
            }
            if (swapin(pid) == -1)
            {
                fileout << pid << " swapin not successfull, ignore success" << endl;
                return -1;
            }
            return 1;
        }
        fileout << pid << " swapin not successfull, ignore success" << endl;
        return -1;
    }
    void exit()
    {
        cout << "exit" << endl;
        MainMemory.clear();
        VirtualMemory.clear();
        mm.clear();
        mprocs.clear();
        procs.clear();
        vprocs.clear();
        nextpid = 1;
    }
};

int main(int argc, char *argv[])
{

    fileout.open(argv[5]);
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " file1 file2 file3 ..." << std::endl;
        return 1; // Indicate an error
    }

    int M, V, P;
    M = stoi(argv[1]);
    V = stoi(argv[2]);
    P = stoi(argv[3]);
    vector<int> MainMemory((M * 1024) / P, -1);
    vector<int> VirtualMemory((V * 1024) / P, -1);
    vector<int> mm(M * 1024 + 1, -9789);
    vector<int> vm(V * 1024 + 1, -9789);
    int nopm = (M * 1024) / P;
    int nopv = (V * 1024) / P;
    // vector<Process>Pro(9999);
    // Check if the user provided the correct number of arguments
    Commando chandler = Commando(MainMemory, VirtualMemory, mm, vm, P);

    string ofile = "";
    int pid;

    // Process each file
    std::ifstream file(argv[4]);

    // Check if the file was opened successfully
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            string lol = "";
            // cout<<line<<endl;
            string k = "";
            string ins = "";
            int gis = 0;
            int mm = 0;

            for (char c : line)
            {
                if (c == ' ')
                {
                    if (!gis)
                    {
                        gis = 1;
                        ins = k;
                        // cout<<ins<<" ";
                        k = "";
                    }
                    else
                    {
                        if (ins == "load")
                        {
                            lol = lol + k + " ";
                            // cout<<k<<endl;
                            // cout<<lol<<endl;
                            // cout<<"load "<<k;
                            k = "";
                        }
                        else if (ins == "run")
                        {
                            // cout<<"run "<<k;
                            k = "";
                        }
                        else if (ins == "kill")
                        {
                            // cout<<"kill "<<k;
                            k = "";
                        }
                        else if (ins == "pte")
                        {
                            // cout<<"pte "<<k;
                            pid = stoi(k);
                            k = "";
                        }
                        else if (ins == "print")
                        {
                            // cout<<"print "<<k;
                            mm = stoi(k);
                            k = "";
                        }
                    }
                }
                else
                {
                    if (c != ' ')
                    {
                        k += c;
                    }
                }
            }
            // cout<<k<<endl;
            if (ins == "load")
            {

                lol = lol + k + " ";
                // cout<<lol<<endl;
                chandler.load(lol);
                fileout << "loaded " << lol << " successfully" << endl;
            }
            else if (ins == "run")
            {
                chandler.run(stoi(k));
                fileout << "ran " << k << " successfully" << endl;
            }
            else if (ins == "pte")
            {
                chandler.pte(pid, k);
                fileout << "pte " << pid << " " << k << " successfully" << endl;
            }
            else if (ins == "kill")
            {
                chandler.kill(stoi(k));
                fileout << "killed " << k << " successfully" << endl;
            }
            else if (ins == "" && k == "listpr")
            {
                // cout<<"hi"<<endl;
                chandler.listpr();
                fileout << "listpr "
                        << "successfully" << endl;
            }
            else if (ins == "pteall")
            {
                chandler.pteall(k);
                fileout << "pteall " << k << " successfully" << endl;
            }
            else if (ins == "swapin")
            {
                int useless = chandler.swapin(stoi(k));
                fileout << "swapin " << k << " successfully" << endl;
            }
            else if (ins == "swapout")
            {
                int useless = chandler.swapout(stoi(k));
                fileout << "swapout " << k << " successfully" << endl;
            }
            else if (ins == "print")
            {
                chandler.print(mm, stoi(k));
                fileout << "print " << mm << " " << k << " successfully" << endl;
            }
            else if (k == "exit")
            {
                chandler.exit();
                fileout << "exit"
                        << " successfully" << endl;
            }
            // std::cout << line << std::endl; // Do something with the line
        }
    }
    else
    {
        std::cerr << "Unable to open file: " << argv[5] << std::endl;
    }

    return 0; // Indicate success
}