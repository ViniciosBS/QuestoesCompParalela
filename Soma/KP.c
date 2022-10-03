int max(int a, int b) { return (a > b)? a : b; }
static int knapSack(long int W, long int N, int wt[], int val[]){
	int *K = (int*) malloc((W+1)*sizeof(int));
	int *Kp = (int*) malloc((W+1)*sizeof(int));
	long int i, w;
	for (i = 0; i <= N; i++)
	{
		#pragma omp parallel for
		for (w = 0; w <= W; w++)
		{
			if (i==0 || w==0)
				K[w] = 0;
			else if (wt[i-1] <= w)
				K[w] = max(val[i-1] + Kp[w-wt[i-1]], Kp[w]);
			else
				K[w] = Kp[w];
		}
		int *tmp = Kp;
		Kp = K;
		K = tmp;
	}

	int result = K[W];
	free(K);
	free(Kp);
	return result;
}
