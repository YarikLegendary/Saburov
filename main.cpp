#include <fstream>
#include <iomanip>

unsigned const N = 100;

void abs(int a){
  int b = 0;
    if (a < 0){
        b = -a;
    }
    else{
    b = a;
    }
}

bool calcpoints(unsigned* size, fstream* f, fstream* g, fstream* h) {
    float p;
    int points = 0; // ��������� ����� ��-�����
    int colms = 0;
    char s = '!';

    *f >> p; // ������ �������� ���������� �����
    if (f->eof() || p <= 0 || p > float(int(p))) { //�������� �� ����� �������� ���������� �����
        *g << "Error\n";
        *h << "Error\n";
        return 0;
    }

    *size = p; // ����������� ������ ����� � ������ ������ � size

    while (!f->eof() && points < *size) { // � ����� ���������� �� �������

        for (;;) { // � ���� ���� ���������� �� ������ � ��������

            *f >> p;

            if (!f->eof()) {
                colms++;
                *f << resetiosflags(ios::skipws); // ������ �������
                do {
                    *f >> s;
                } while (!f->eof() && (s == ' ' || s == '\t'));
                *f << setiosflags(ios::skipws); // �� ������ �������
            }
            if ((f->eof()) || (s == '\n')) {

                if (colms == 2) {
                    points++;
                }
                if (colms > 2) {
                    points++;
                    s = '\n';
                }
                colms = 0; // ���������� �������
                break;
            }
            f->seekg(-1, ios::cur);
        }
    }

    *h << "�������� ���������� �����: " << *size << "\n" << endl;

    if (points < *size) {
        *size = points;
    }
    if (*size > N) {
        *size = N;
    }

    return 1;
}
void InpPoints(unsigned size, float** arr, fstream* h) {
    fstream f; fstream g; char s; int k = 0;
    f.open("koordinaty.txt", ios::in);
    if (!f.is_open()) {
        g << "������ �������� �����\n";
        return;
    }
    f >> **arr;

    f << resetiosflags(ios::skipws); // ������ �������
    do {
        f >> s;
    } while (!f.eof() && s != '\n');

    for (unsigned i = 0; i < size; i++) {

        k++;
        *h << "������ " << k << "-� ������ ..... ";

        f << resetiosflags(ios::skipws); // ������ �������
        do {
            f >> s;
            if (s == '\n') {
                *h << "c����� �����" << endl;
                k++;
                *h << "������ " << k << "-� ������ ..... ";
                s = ' ';
            }
        } while (!f.eof() && (s == ' ' || s == '\t'));
        f.seekg(-1, ios::cur);

        f >> arr[i][0];

        do {
            f >> s;
        } while (!f.eof() && (s == ' ' || s == '\t'));

        if (f.eof() || s == '\n') {
            *h << "���������� y � ������ ����������" << endl;
            i--;
        }
        else
        {
            f.seekg(-1, ios::cur);
            f >> arr[i][1];
            f << resetiosflags(ios::skipws); // ������ �������
            do {
                f >> s;
            } while (!f.eof() && s != '\n');
            f << setiosflags(ios::skipws); // �� ������ �������

            *h << i + 1 << " ����� � ������������ " << "(" << arr[i][0] << ";" << arr[i][1] << ")" << " �������" << endl;
        }
    }
    f.close();
}
void Out1(unsigned size, float* arr, fstream* g) {
    for (unsigned j = 0; j < 2; ++j) {
        *g << arr[j] << " ";
    }
    *g << endl;
}
void Out2(unsigned size, float** arr, fstream* g) {
    *g << "���������� �����: " << size << endl;
    *g << "\n";
    for (unsigned i = 0; i < size; i++) {
        Out1(size, arr[i], g);
    }
}
void OutProtokol_1(unsigned size, float* arr, fstream* h) {
    *h << "(";
    for (unsigned j = 0; j < 2; ++j) {
        *h << arr[j] << ";";
    }
    h->seekg(-1, ios::cur);
    *h << ")\n";
}
void OutProtokol_2(unsigned size, float** arr, fstream* h) {
    *h << "\n��������� " << size << " �����: " << endl;
    for (unsigned i = 0; i < size; i++) {
        OutProtokol_1(size, arr[i], h);
    }
}

void CreateVectors(unsigned size, float** arr, float** vectors, int vectorIndex, fstream* h) {

    *h << "\n�������, ���������� �� ��� �����:\n";

    // ������� ���� ��� ����� ��� ���������� �������� � �� ����� � ����
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            /*if (!(arr[j][0] - arr[i][0] == 0 && arr[j][1] - arr[i][1] == 0)) {*/
            vectors[vectorIndex][0] = arr[j][0] - arr[i][0]; // x-���������� �������
            vectors[vectorIndex][1] = arr[j][1] - arr[i][1]; // y-���������� �������
            *h << "������ " << vectorIndex + 1 << " ����� ������� " << i + 1 << " � " << j + 1
                << ": (" << vectors[vectorIndex][0] << ", " << vectors[vectorIndex][1] << ")\n";

            vectorIndex++;
            //}
            //else {
            //    *h << "������� ������ � �����: " << "(" << arr[i][0] << ";" << arr[i][1] << ")" << endl;
            //}
        }
    }
}

int Process(unsigned size, float** arr, float** vectors, int vectorIndex, int VectorsSIZE, fstream* h, fstream* g) {
    double eps = 1e-7;

    unsigned int** ArrOfEquals = new unsigned int* [VectorsSIZE + 1];// �������� ������������� ���������� ������� 
    for (int i = 0; i < VectorsSIZE + 1; ++i) {
        if (ArrOfEquals)
            ArrOfEquals[i] = new unsigned int[2]; // ������� ������ ��� �����
        else {
            *h << "������ ��������� ������ ��� ���������� �����" << endl;
            break;
        }
    }

    double Skalyar = 0, lengthA = 0, lengthB = 0, cos = 0, maxCos = 0;
    int Index1 = -1, Index2 = -1;
    int parallel = 0, countsize = 0;
    int CountSame = 0, k = 0, CountZero = 0;
    *h << "\n";

    for (int i = 0; i < vectorIndex; ++i) {
        for (int j = i + 1; j < vectorIndex; ++j) {
            Skalyar = vectors[i][0] * vectors[j][0] + vectors[i][1] * vectors[j][1];
            lengthA = sqrt(vectors[i][0] * vectors[i][0] + vectors[i][1] * vectors[i][1]);
            lengthB = sqrt(vectors[j][0] * vectors[j][0] + vectors[j][1] * vectors[j][1]);
            cos = Skalyar / (lengthA * lengthB);

            countsize++;

            if (lengthA * lengthB == 0) {
                *h << "!Warning: ������� ������ " << endl;
                CountZero++;
            }
            else {
                if (abs(cos) < 1 + eps && abs(cos) > 1 - eps) {
                    *h << "!Warning: ������� " << i + 1 << " � " << j + 1 << " ����������� -> ������ ����������� -> ���� ����� 0 ��������" << endl;
                    parallel++;
                    continue;
                }

                *h << "������� (�� ������) ����� ��������� " << i + 1 << " � " << j + 1 << " �����: " << setprecision(4) << abs(cos) << endl;

                if (abs(cos) >= maxCos) {
                    if (abs(cos) == maxCos) {
                        CountSame++;
                        ArrOfEquals[k][0] = i;
                        ArrOfEquals[k][1] = j;
                        k++;
                        continue;
                    }
                    maxCos = abs(cos);
                    CountSame = 0;
                    Index1 = i;
                    Index2 = j;
                }
            }
        }
    }
    if (CountZero == countsize) {
        *h << "\n!Warning: ��� ������� �������" << endl;
        *g << "\n!Warning:  ��� ������� �������" << endl;
        return 0;
    }

    if (parallel == countsize) {
        *h << "\n!Warning: ��� ������ �����������" << endl;
        *g << "\n!Warning: ��� ������ �����������" << endl;
        return 0;
    }
    if (CountZero + parallel == countsize) {
        *h << "\n!Warning: ����� ���" << endl;
        *g << "\n!Warning: ����� ���" << endl;
        return 0;
    }
    if (CountSame == 0) {
        *h << "\n���������� �� ������ ������� ���� ����� ��������� " << Index1 + 1 << " � " << Index2 + 1
            << ": " << setprecision(4) << maxCos << endl;
        *h << "���� ����� ������� ����� " << setprecision(3) << (180 - (acos(maxCos) * 180 / 3.1415)) << " ����." << endl;
        *g << "\n���������� �� ������ ������� ���� ����� ��������� " << Index1 + 1 << " � " << Index2 + 1
            << ": " << setprecision(4) << maxCos;
        *g << ". ���� ����� ������� ����� " << setprecision(3) << (180 - (acos(maxCos) * 180 / 3.1415)) << " ����." << endl;
    }
    if (CountSame != 0) {
        *h << "\n���������� �� ������ ������� ���� ����� ���������: " << Index1 + 1 << " � " << Index2 + 1 << endl;

        k = 0;
        for (int i = 0; i < CountSame; i++) {
            *h << "���������� �� ������ ������� ���� ����� ���������: " << ArrOfEquals[k][0] + 1 << " � " << ArrOfEquals[k][1] + 1 << endl;
            k++;
        }
        *h << "������� (�� ������) �����: " << setprecision(4) << maxCos;
        *h << ". ���� ����� ������� ����� " << setprecision(3) << (180 - (acos(maxCos) * 180 / 3.1415)) << " ����." << endl;
        *g << "\n���������� ������� (�� ������) �����: " << setprecision(4) << maxCos;
        *g << ". ���� ����� ������� ����� " << setprecision(3) << (180 - (acos(maxCos) * 180 / 3.1415)) << " ����." << endl;
    }

    int countpoint1_1 = 0, countpoint1_2 = 0, countpoint2_1 = 0, countpoint2_2 = 0;

    for (int a = 0; a < size; ++a) {
        for (int b = a + 1; b < size; ++b) {
            if ((abs(vectors[Index1][0] - (arr[b][0] - arr[a][0])) <= eps) &&
                (abs(vectors[Index1][1] - (arr[b][1] - arr[a][1])) <= eps)) {
                countpoint1_1 = a;
                countpoint1_2 = b;
                goto GNEXT1;
            }
        }
    }
GNEXT1:
    for (int a = 0; a < size; ++a) {
        for (int b = a + 1; b < size; ++b) {
            if ((abs(vectors[Index2][0] - (arr[b][0] - arr[a][0])) <= eps) &&
                (abs(vectors[Index2][1] - (arr[b][1] - arr[a][1])) <= eps)) {
                countpoint2_1 = a;
                countpoint2_2 = b;
                goto GNEXT2;
            }
        }
    }
GNEXT2:
    *h << "\n���������� ���� ���������� ����� ������, ������������ ������� ";
    *h << countpoint1_1 + 1 << " � " << countpoint1_2 + 1 << ", � ������ ����� ������� ";
    *h << countpoint2_1 + 1 << " � " << countpoint2_2 + 1 << endl;
    *g << "\n���������� ���� ���������� ����� ������, ������������ ������� ";
    *g << countpoint1_1 + 1 << " � " << countpoint1_2 + 1 << ", � ������ ����� ������� ";
    *g << countpoint2_1 + 1 << " � " << countpoint2_2 + 1 << endl;
        for (int i = 0; i < CountSame; ++i) {

            for (int a = 0; a < size; ++a) {
                for (int b = a + 1; b < size; ++b) {
                    if ((abs(vectors[ArrOfEquals[i][0]][0] - (arr[b][0] - arr[a][0])) <= eps) &&
                        (abs(vectors[ArrOfEquals[i][0]][1] - (arr[b][1] - arr[a][1])) <= eps)) {
                        countpoint1_1 = a;
                        countpoint1_2 = b;
                        goto NEXT1;
                    }
                }
            }
            NEXT1:
            for (int a = 0; a < size; ++a) {
                for (int b = a + 1; b < size; ++b) {
                    if ((abs(vectors[ArrOfEquals[i][1]][0] - (arr[b][0] - arr[a][0])) <= eps) &&
                        (abs(vectors[ArrOfEquals[i][1]][1] - (arr[b][1] - arr[a][1])) <= eps)) {
                        countpoint2_1 = a;
                        countpoint2_2 = b;
                        goto NEXT2;
                    }
                }
            }
            NEXT2:
            *h << "���������� ���� ���������� ����� ������, ������������ ������� ";
            *h << countpoint1_1 + 1 << " � " << countpoint1_2 + 1 << ", � ������ ����� ������� ";
            *h << countpoint2_1 + 1 << " � " << countpoint2_2 + 1 << endl;
            *g << "���������� ���� ���������� ����� ������, ������������ ������� ";
            *g << countpoint1_1 + 1 << " � " << countpoint1_2 + 1 << ", � ������ ����� ������� ";
            *g << countpoint2_1 + 1 << " � " << countpoint2_2 + 1 << endl;
        }
}

int main() {

    setlocale(LC_ALL, "RU");
    unsigned size = 0;

    fstream f("koordinaty.txt", ios::in);
    fstream g("output.txt", ios::out);
    fstream h("protokol.txt", ios::out);

    if (!f.is_open()) {
        g << "������ �������� ����� � ������������\n";
    }
    if (!h.is_open()) {
        g << "������ �������� ���������\n";
        return 0;
    }
    if (!calcpoints(&size, &f, &g, &h)) {
        g << "������ ������ �����\n";
        return 0;
    }

    float** arr = new float* [size];// �������� ������������� ���������� �������
    for (int i = 0; i < size; ++i) {
        if (arr)
            arr[i] = new float[2]; // ������� ������ ��� �����
        else {
            g << "������ ��������� ������ ��� ���������� �����" << endl;
            break;
        }
    }

    f.clear();
    f.seekg(0);
    InpPoints(size, arr, &h);
    if (f.is_open())
        f.close();

    Out2(size, arr, &g);
    OutProtokol_2(size, arr, &h);

    int VectorsSIZE = size * (size - 1) / 2; // ������������ ���������� ��������

    float** vectors = new float* [VectorsSIZE]; // ������������ ������ ��� �������� ��������
    for (int i = 0; i < VectorsSIZE; ++i) {
        if (vectors)
            vectors[i] = new float[2]; // ������� ������� ����� ����� 2 ����������
        else {
            g << "������ ��������� ������ ��� ���������� ��������" << endl;
            break;
        }
    }

    int vectorIndex = 0;
    CreateVectors(size, arr, vectors, vectorIndex, &h);
    Process(size, arr, vectors, VectorsSIZE, VectorsSIZE, &h, &g);

    if (g.is_open())
        g.close();
    if (h.is_open())
        h.close();

    // �������� ������ ��� ���������� �����
    for (int i = 0; i < size; ++i) {
        delete[] arr[i];
    }
    delete[] arr;

    // ������������ ������ ��� �������
    for (int i = 0; i < VectorsSIZE; ++i) {
        delete[] vectors[i];
    }
    delete[] vectors;

    return 0;
}

