# include < windows.h >
# include < stdio.h >
# include < conio.h >
#include <time.h>
#include <math.h>


#define NUMBER_THREADS 10

#define M_PI 3.14159265359


BOOL isFinishedAll;
clock_t start, end;
double cpu_timed_is_used;


typedef struct // tablica zawiera dane , ktore otrzymaja watki
{
	long int thread_id;
	long int* matrix;
	long int start_index;
	long int end_index;
} ThreadData;



DWORD WINAPI IncrementMatrix(void* data) {
	ThreadData* thread_data = (ThreadData*)data;

	for (int i = thread_data->start_index; i < thread_data->end_index; ++i) {
		(thread_data->matrix)[i] = (thread_data->matrix)[i] * 2 + (thread_data->matrix)[i] % 2;
	}

	return 0;
}


int main() {
	int MATRIX_SIZE = 100000000;

	int * matrix = (int*)malloc(MATRIX_SIZE * sizeof(int));

	for (int i = 0; i < MATRIX_SIZE; i++) {
		matrix[i] = 4324;
	}
	

	for (int f = 1; f < 16; f++) {

		HANDLE* threads = malloc(sizeof(HANDLE) * (f));
		ThreadData* thread_data = malloc(sizeof(ThreadData)* f);

		long long int chunk_size = MATRIX_SIZE / f;
		long long int remainder = MATRIX_SIZE % f;

		long long int start_index = 0;
		long long int end_index = 0;

		
		

		for (int i = 0; i < f; i++) {
			start_index = end_index;
			end_index = start_index + chunk_size + (i < remainder ? 1 : 0);

			thread_data[i].thread_id = i;
			thread_data[i].matrix = matrix;
			thread_data[i].start_index = start_index;
			thread_data[i].end_index = end_index;

			threads[i] = CreateThread(NULL, 0, IncrementMatrix, &thread_data[i], 0, NULL);
			if (threads[i] == NULL) {
				fprintf(stderr, "B³¹d podczas tworzenia w¹tku.\n");
				return 1;
			}

		}
		
		
		start = clock();
		WaitForMultipleObjects(f, threads, TRUE, INFINITE);

		end = clock();

		double cpu_timed_is_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("%i Czas %f\n", f, cpu_timed_is_used);

	/*	for (int i = 0; i < MATRIX_SIZE; ++i) {
			printf("%d ", matrix[i]);
		}
		printf("\n");*/



		for (int i = 0; i < f; ++i) {
			CloseHandle(threads[i]);
		}

	}



	return 0;
}
