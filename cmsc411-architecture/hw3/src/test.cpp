#include <iostream>
using namespace std;

int main()
{
    const int len = 4;

    int A[len] = {1, 2, 3, 4};
    int B[len] = {9, 8 , 7, 6};

    // multiply A * B
    int res[1] = {0};                           // result array
    for (int i=0; i<len; i++) {
        res[0] += A[i] * B[i];
    }
    cout << "A * B result: [" << res[0] << "]" << endl;


    // multiply B * A
    int arr[len][len];                          // result array

    cout << "\nB * A result: " << endl;
    for (int i=0; i<len; i++) {                 // iterate over each row of B

        for (int j=0; j<len; j++) {             // now iterate over each col of A
            arr[i][j] = B[i] * A[j];
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
