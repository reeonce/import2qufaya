### what is this

import2qufaya is a command line tool that converts wechat bill into 圈子账本's format.


### how to build

```bash
git clone xxx
mkdir build
cd build
cmake ..
make
```

### how to use?

1. download the wechat bill from wechat app.

2. by following *map.csv.sample*'s format, create your own map.

3. execute the command `./im2qufaya xxx.csv wechat.csv`

4. save wechat.csv.out.csv as wechat.xlsl, then import wechat.xlsl via [https://www.jizhangapp.com/pushMore](https://www.jizhangapp.com/pushMore)
