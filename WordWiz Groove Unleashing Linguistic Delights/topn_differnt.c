int findTopN(Dict d, WFreq *wfs, int n)
{
   // TODO
   int s = dofindTopN(d->tree, wfs, n, 0, 0);
   int k = (s%n);
   for (int f = k; f >= 0; f--) {
      printf ("%7d %s\n", wfs[k].freq, wfs[k].word);
   }
   for (int e = k; e < n; e++) {
      printf ("%7d %s\n", wfs[k].freq, wfs[k].word);
   }
   return k;
}

int dofindTopN (Link d, WFreq *wfs, int n, int i, int j) {
   if (d == NULL) {
      return;
   }
   dofindTopN(d->left, wfs, n, i, j);
   if (i == 0) {
      if (j != 0) {
         if (d->data.freq >= wfs[i].freq) {
            wfs[i] = d->data;
         }
      }
      else {
         wfs[i] = d->data;
      }
      i++;
   }
   
   else if(i < n) { 
      if (d->data.freq >= wfs[i-1].freq)  {
         wfs[i] = d->data;
         i++;
      }
   }
   else if (i >= 10) {
      i = 0;
      j++;
   } 
   dofindTopN(d->right, wfs, n, i, j);
   return j;

}
if (nWords > num_of_words) {
      results = (WFreq*)calloc(num_of_words, sizeof(*results));
      results[num_of_words-1] = *most_frequent;
      findTopN(word_read, results, num_of_words);
   }
   else {
      results = (WFreq*)calloc(nWords, sizeof(*results));
      results[nWords-1] = *most_frequent;
      findTopN(word_read, results, num_of_words);
   }