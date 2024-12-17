
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
using namespace std;

const unsigned n = 20;

bool calcsize(fstream* f, unsigned* a, unsigned* k);
void inp(fstream* f, unsigned a, float** A);
void out1(float** A, unsigned a, fstream& f);
void out2(fstream* f, unsigned a, float* A);
float process(float** A, unsigned size, unsigned k);

bool calcsize(fstream* f, unsigned* a, unsigned* k) {
    float p; int rows = 0, columns = 0; char s = '!';

    *f >> p;
    if (f->eof() || p <= 0 || p > float(int(p))) { //�������� �� ����� �������� ������� �������
        cout << "�������� ������� ������� ������ ���� ����� �������������.\n";
        return 0;
    }
    *a = p;

    *f >> p;
    if (f->eof() || p <= 0 || p > float(int(p))) { //�������� �� ����� �������� ���������
        cout << "�������� ��������� k ������ ���� ����� �������������.\n";
        return 0;
    }
    *k = p;

    while (!f->eof() && rows < *a) {
        for (;;) {
            *f >> p;
            if (!f->eof()) {
                columns++;
                *f << resetiosflags(ios::skipws);
                do {
                    *f >> s;
                } while (!f->eof() && (s == ' ' || s == '\t'));
                *f << setiosflags(ios::skipws);
            }
            if ((f->eof()) || (s == '\n')) {
                if (columns != 0) {
                    rows++;
                    if (columns < *a) {
                        *a = columns;
                    }
                }
                columns = 0;
                break;
            }
            f->seekg(-1, ios::cur);
        }
    }
    if (rows < *a) {
        *a = rows;
    }
    if (*a > n)
        *a = n;
    return 1;
}

void inp(fstream* f, unsigned a, float** A) {
    char s;
    *f >> **A >> **A;
    for (unsigned i = 0; i < a; i++) {
        for (unsigned j = 0; j < a; j++) {
            *f >> A[i][j];
        }
        *f << resetiosflags(ios::skipws);
        do {
            *f >> s;
        } while (!f->eof() && s != '\n');
        *f << setiosflags(ios::skipws);
    }
}

void out2(fstream* f, unsigned size, float* A) {
    for (unsigned j = 0; j < size; ++j) {
        *f << A[j] << ' ';
    }
    *f << endl;
}

void out1(float** A, unsigned size, fstream& f) {
    /*f << "������� ������� " << size << 'x' << size << endl;*/
    for (unsigned i = 0; i < size; i++) {
        out2(&f, size, A[i]);
    }
}

float process(float** A, unsigned size, unsigned k) {
    float max = A[k - 1][0];
    unsigned x = 0, y = k;
    --k; --size;
    while (y)
    {
        --y;
        if (A[y][x] > max)
            max = A[y][x];
        ++x;
    }
    x = 0; y = k;
    while (y < size)
    {
        if (A[y][x] > max)
            max = A[y][x];
        ++x; ++y;
    }
    while (y > k)
    {
        if (A[y][x] > max)
            max = A[y][x];
        --y;
    }
    while (x <= size)
    {
        if (A[y][x] > max)
            max = A[y][x];
        ++x;
    }
    return max;
}

int main() {
    setlocale(LC_ALL, "RU");

    cout << "�������:\n����� ���������� ������� � ������� ������� n*n\n"
         << "�����: ������� �������, 4352\n"
         << "������: 1.1.\n";

    unsigned size = 0, // ������ �������
        k = 0, // ��������
        A_alloc = 0; // ��� ������� ����� �������� ������
    bool success = 1; // ���������� ���������� ���������� ����� ���������
    float** A = NULL; // �������

    fstream f("Matrix.txt", ios::in); // ��������� ������ � �������

    if (!f.is_open()) {
        cout << "������ �������� �����\n";
        success = 0;
    }
    if (success && !calcsize(&f, &size, &k)) {
        cout << "������ ������ �����.\n";
        success = 0;
    }
    if (success)
    {
        A = new float* [size];
        if (A)
            for (; A_alloc < size;) {
                if (A[A_alloc] = new float[size])
                    ++A_alloc;
                else
                {
                    cout << "������ ��������� ������ ��� �������." << endl;
                    success = 0;
                    break;
                }
            }
    }
    if (success)
    {
        f.clear();
        f.seekg(0);
        inp(&f, size, A);
        f.close();
        f.open("out.txt", ios::out);
        if (!f.is_open()) {
            cout << "������ �������� ��������� �����.\n";
            success = 0;
        }
    }
    if (success) {
        out1(A, size, f);
        f << "������������ �������� � �������������� �������: " << process(A, size, k) << endl;
        cout << "���������� �������� � ���� out.txt" << endl;
    }

    if (f.is_open())
        f.close();
    for (; A_alloc;) {
        delete[] A[--A_alloc];
    }
    if (A)
        delete[] A;

    return success ? 0 : 1;
}