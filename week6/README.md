## 宿題　malloc challenge!

> freelist malloc を更新してメモリ使用率と速度をUpさせる  
> First fit を best fit , worst fit などに変更してみて、その比較を行う

## 解答

fleelist malloc をこのように変更しました。

```
  // Best-fit: Find the min free slot the object fits.
  my_metadata_t *min_metadata = NULL;
  my_metadata_t *prev_min_metadata = NULL;

  while(metadata){
    prev = metadata;
    metadata = metadata->next;
    if(!metadata) break;

    if(metadata->size >= size){
      if(min_metadata == NULL){
        prev_min_metadata = prev;
        min_metadata = metadata;
      }
      else if(metadata->size < min_metadata->size){
        prev_min_metadata = prev;
        min_metadata = metadata;
      }
    }
  }
  prev = prev_min_metadata;
  metadata = min_metadata;
```

```
  // Worst-fit: Find the max free slot the object fits.
  my_metadata_t *max_metadata = NULL;
  my_metadata_t *prev_max_metadata = NULL;

  while(metadata){
    prev = metadata;
    metadata = metadata->next;
    if(!metadata) break;

    if(metadata->size >= size){
      if(max_metadata == NULL){
        prev_max_metadata = prev;
        max_metadata = metadata;
      }
      else if(metadata->size > max_metadata->size){
        prev_max_metadata = prev;
        max_metadata = metadata;
      }
    }
  }
  prev = prev_max_metadata;
  metadata = max_metadata;
 ```
 
 実行結果はこのようになりました。
 
 | | Challenge1 | Challenge2 | Challenge3 | Challenge4 | Challenge5 |
|:---|:---:|---:|:---:|:---:|:---:|
| first fit | 10ms / 70% | 7ms / 40% | 80ms / 7% | 21782ms / 15% | 14884ms / 15% |
| best fit |971ms / 70% | 277ms / 40% | **311ms / 50%** | **7015ms / 71%** | **4047ms / 74%** |
| worst fit | 984ms / 69% | 294ms / 40% | **44443ms 3%** | **899567ms / 6%** | **714998ms / 6%** |
 
 
特に違いが顕著だったのがChallenge3-5で、best fitは大幅にUtilizationを増やしtimeも短縮されました。  
(timeに関して、全ての空き領域を見る必要があるためデータ量の少ないChallenge1-3は逆に仕事が増えtimeも増えています)  
一方でworst fitはfirst fitよりもUtilizationを減らしたほか、timeの結果がかなり悪くなりました。
