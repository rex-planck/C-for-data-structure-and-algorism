# 25/12/17  
## understanding about `Class queue`  
实现数组类型的队列类操作，需要以下几步  
  1. 定义一个队列类(Cardqueue)  
    1.1 private：
      - int* `array` for dynamic space allocation  
      - int `head` for array first element pointer  
      - int `tail` for array last element pointer  
      - int `count` for noting the current array size  
      - int `capacity` for allocating array size
    
    1.2 public: for initializing array  
      - set `Cardqueue`:  
        - int `n`  
        - capacity = `n+1`  
        - array = new int [capacity]  
        - count/head/tail = 0  
        







