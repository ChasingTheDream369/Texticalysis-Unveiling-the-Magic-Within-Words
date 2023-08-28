int findTopN(Dict d, WFreq *wfs, int n)
{
   // TODO
   dofindTopN(d->tree, wfs, n);
   return n;
   
}

void dofindTopN (Link d, WFreq *wfs, int n) {
    if (d == NULL) {
      return;
    }
    dofindTopN(d->left, wfs, n);
    int j = n-1;
    while (d->data.freq <= wfs[j].freq && j < n) j--;
    for (int i = 0; i < j; i++) 
        wfs[i] = wfs[i+1];
        wfs[j] = d->data;
    dofindTopN(d->right, wfs, n);


   

}