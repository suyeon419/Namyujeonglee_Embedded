# 옴짝달싹

본 작품은 사용자 ~~~~ 구현을 목표로 함.


## 👨‍🏫 하드웨어 구성


![Frame 191 (1)](https://github.com/user-attachments/assets/3f92ae35-6ffe-4b38-b1aa-0e05885d6056)


**타이머의 내부 배치도는 프로젝트의 구성 요소와 배치를 시각적으로 나타낸다.** 

각 부품은 다음과 같은 역할을 수행한다. 각 층을 명확히 구분하기 위해 부품을 안정적으로 지탱할 수 있는 바닥이 설계되어 있다. 아래에서부터 차례대로 라즈베리파이가 배치된다. 전체 시스템의 중앙 처리 장치로, 다른 구성 요소들의 제어를 담당한다. 오른쪽 하단에 전원 단자의 홈이 존재한다. 이는 라즈베리파이 시스템에 전원을 공급하는 역할을 한다. 중앙에는 브레드보드가 배치되며, 각 부품의 회로 연결을 담당한다. 보드에 연결되는 내부 구성 요소로 스텝모터, RGB LED가 있다. 스텝모터는 타이머의 핵심 부품으로 시간이 흐르는 것을 사용자에게 시각적으로 알리기 위해 이미지와 같이 동그란 홈을 통해 외부로 돌출된다. 점멸용 RGB LED도 스텝모터와 동일한 방식으로 디자인되어 있으며, 사용자가 공부 시간과 쉬는 시간을 명확히 구별하고, 타이머의 시간이 흐르는 것을 시각적으로 인지할 수 있도록 하는 데 중점을 두었다. 무드등용 RGB LED는 타이머 상단에 배치되는 조도센서의 외부 밝기 측정 값에 따라 적절한 밝기를 유지하도록 한다. 타이머의 뒷면은 투명한 아크릴로 제작되어 무드등의 빛이 잘 보이도록 디자인되었다. 또한, 사용자가 쉽게 조작할 수 있도록 위에서 아래로 슬라이드할 수 있는 홈이 마련되어 있어, 부품을 쉽게 빼고 넣을 수 있도록 설계되었다.

![Frame 192 (1)](https://github.com/user-attachments/assets/d0f5b168-cd6b-4b61-b3d1-9f1065586a56)

**다음은 타이머의 상단 배치도로 각 부품의 구성 요소와 배치를 시각적으로 나타낸다.**

도 센서: 조도센서는 왼쪽에 위치하며, 주변의 밝기를 감지하여 무드등의 밝기를 자동으로 조절하는 역할을 한다. 이미지와 같이 디자인한 이유는 조도 센서를 안정적으로 고정시키는 역할을 하며, 외부 밝기를 측정하는 데 있어 오차를 최소화하기 위해 디자인했다. 
초음파 센서는 앞쪽에 배치되어 있으며, 사용자가 자리에 앉아있는지 판별할 때 사용된다. 구멍은 초음파 센서를 안정적으로 고정시키는 역할을 하며, 센서와 연결된 전선을 외부로 쉽게 빼낼 수 있도록 디자인되었다. 시간 조절용 스위치는 중앙 하단에 위치하며, 사용자가 타이머의 시간을 조절할 수 있는 버튼이다. 브레드보드에 연결된 버튼이 돌출될 수 있도록 구멍이 설계되어 있으며, 간편한 조작을 통해 원하는 공부 시간을 쉽게 설정할 수 있다. 


![Frame 193 (1)](https://github.com/user-attachments/assets/e38c0c73-41ca-4788-989e-63f34809528f)

**초음파센서의 파츠 디자인이다.**

초음파센서가 보다 안정적으로 배치될 수 있도록 부품 사이즈에 맞게 디자인되어 있다.

![Frame 194](https://github.com/user-attachments/assets/3505b28b-9d22-48c0-b468-babaf5525574)

**스텝모터에 시계바늘 파츠 디자인이다.**

타이머 컨셉에 맞게 시계바늘 파츠로 모터의 돌출 부분과 사이즈가 맞도록 디자인되어 있다.
