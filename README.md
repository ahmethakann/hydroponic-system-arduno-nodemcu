[ahmet_hakan_özgüven_tez_final-son.pdf](https://github.com/ahmethakann/hydroponic-system-arduno-nodemcu/files/13364686/ahmet_hakan_ozguven_tez_final-son.pdf)
[ahmet_hakan_özgüven_poster_sunumu_171802048.pdf](https://github.com/ahmethakann/hydroponic-system-arduno-nodemcu/files/13364687/ahmet_hakan_ozguven_poster_sunumu_171802048.pdf)


Yapılan topraksız tarım sisteminin izleme ve kontrolünü sağlayabilmek ve sensörlerden okunan verileri kayıt edebilmek için Arduino ve ESP tabanlı IOT bir sistem tasarlandı. Bu sistemde, ESP8266 mikroişlemci sayesinde internete bağlanılarak tarih ve saat değerleri NTP Server’dan alınabilmektedir. Yine internet bağlantısı ile Google’ın bir hizmeti olan Firebase veritabanı üzerine hem saniye başı güncel verilerin kaydedilmesi hem de uzaktan röle kontrollerinin yapılabilmesi sağlanmıştır. Sadece wifi bağlantısına ihtiyaç duyan IOT bir sistem tasarlanmıştır.
![image](https://github.com/ahmethakann/hydroponic-system-arduno-nodemcu/assets/66576626/a2eda9c9-9804-44e2-9b42-f84c03d82839)
![image](https://github.com/ahmethakann/hydroponic-system-arduno-nodemcu/assets/66576626/af246989-b24e-4495-9988-968c1c9a8eea)
![image](https://github.com/ahmethakann/hydroponic-system-arduno-nodemcu/assets/66576626/d8fe57b9-19eb-480f-8ac4-945315103ebf)
![image](https://github.com/ahmethakann/hydroponic-system-arduno-nodemcu/assets/66576626/6e6ab732-023a-4737-b8b9-59aef6ee8a65)
![image](https://github.com/ahmethakann/hydroponic-system-arduno-nodemcu/assets/66576626/a7c8e543-8b35-42bf-b0e2-a2e33c3bb185)

Mikrodenetleyiciler, içerisinde bellek, giriş-çıkış ve mikro işlemci üniteleri bulunduran entegre hali getirilmiş programlanabilen elemanlardır. Arduino geliştirme kartı, açık kaynak kodlu olarak geliştirilmiş yazılım ve donanım tabanlı bir elektronik platformdur. Arduino Uno 'nun 14 tane dijital giriş / çıkış pini vardır. Bu pinlerin tamamının lojik seviyesi 5V’dur. Her pin maksimum 40mA giriş ve çıkış akımı ile çalışır. Bunlardan 6 tanesi PWM çıkışı olarak kullanılabilir. Ayrıca 6 adet analog girişi, bir adet 16 MHz kristal osilatörü, power jakı (2.1mm), ICSP başlığı ve reset butonu bulunmaktadır. Arduino Uno bir mikrodenetleyiciyi desteklemek için gerekli bileşenlerin hepsini içerir.  Arduino Uno, üzerinde Atmega328 mikrodenetleyici, güç regülatörü, USB bağlantı gibi bileşenler bulunduran, seri-USB dönüştürücü sayesinde hem seri port üzerinden iletişim kurabilirken hem de programlanabilmektedir. Arduino’da kullanılan dil Arduino’nun temelini oluşturan processing programlama dilidir. Bu dil C++ ve C alt yapısına sahiptir. 
Arduino Uno’da normalde bir tane seri port bulunmaktadır fakat SoftwareSerial kütüphanesi kullanılarak bu sayı yazılımsal olarak arttırılabilir. Atmega328 ayrıca I2C ve SPI portları da sağlamaktadır. Arduino IDE içerisinde yer alan Wire kütüphanesi I2C kullanımını, SPI kütüphanesi de SPI haberleşmesini sağlamak için kullanılır.
Harici güç kaynağı olarak 6-20V arası limit değerleri bulunmaktadır. Önerilen harici besleme gerilimi ise 7-12 V arasıdır. Bunun sebebi 7V altındaki gerilimin stabil çalışmayıp, 12V üzeri gerilimin de aşırı ısınma sebebi olabilmesidir. Kart üzerinde bulunan regülatör sayesinde 7-12V arası gerilim 5V’a düşürülür ve kart bu şekilde çalışır. Projemizde 220 VAC’yi 12 VDC ve 2 Ampere çeviren adaptör kullanılmıştır.
Arduino için yazılan kodda SoftwareSerial kütüphanesi ESP8266 ile yazılımsal seri haberleşme yapabilmek için kullanılmıştır. Arduinonun 10. pini RX 11. Pini ise TX olarak tanımlanmıştır. 
ESP8266’ya veriler su seviyesi, pompa rölesinin durumu ve LED rölesinin durumu olmk üzere 3 tanedir. ESP8266’dan alınan veriler ise Epoch time yani 1970 yılının başını 0 olarak kabul eden ve her saniye artan bir zaman bilgisi, Pompa ve LED rölelerini internet üzerinden kontrol edildiğinde gelen verilerin Arduino’ya aktarılmasını sağlayan toggle verileri olmak üzere toplam 3 tanedir. 
Tüm veriler String veri biçimde yollanmıştır ve yollanan verilerin hata kontrolünü sağlamak için Parse fonksiyonu oluşturulmuştur. Bu fonksiyonda String verinin ardına bir harf eklenerek karşı tarafa gönderilir ve alıcı taraf bu ayırma harflerini bildiğinden verileri parçalara böler. Bu sayede tek seferde birden fazla veri bloğu gönderilip alınabilmektedir ve hatalı veri iletişimi engellenmektedir.
Deney sırasında kodun takıldığı görülüp devirdaim motorunun çalışmadığı gözlemlenmiştr. Bu durumun önüne geçebilmek için Arduino’nun Watchdog Timer özelliği koda eklenmiştir. Bu özelliğin eklendiği döngü fonksiyonlarında Watchdog timer resetlenmezse Arduino kendini sıfırlayacak ve kod baştan çalışacaktır. Wdt reset süresi 4 saniye olarak ayarlanmıştır yani kod 4 saniye boyunca bir yerde takılıp wdt’yi resetlemezse watchdog timer aktive olarak kodu baştan başlatacaktır.
İnternet olduğu durumda pompa ve led röleleri zaman bilgisini ESP’den aktarılan epoch time zaman bilgisini kullanarak çalışmaktadır. Pompanın 5 dakika çalışıp 10 dakika durması ve led’in saat 6 ve 22 arasında çalışıp geri kalan zamanda durması beklenmektedir.
İnternet olmadığı durumlarda ise zaman tespiti yapabilmek için Arduino’nun dahili kesme zamanlayıcısı (timer interrupt) kullanılmaktadır. Bu kesme zamanlayıcısı Arduino kartında bulunan 16 MHz’lik kristal osilatörü kullanarak yüksek hassasiyette zaman sayması yapabilmektedir. Ayrıca void loop() fonksiyonunda delay işlemi uygulansa bile zaman kesmesi fonksiyonu mikroişlemci’nin ayrı bir bölgesinde enerji kesintisi olmadığı taktirde sürekli çalışmaktadır. 
Yukarıda yapılan ayarlar ile koda her saniye kesme işlemi uygulanacak ve kesme fonksiyonu içerisindeki kod çalışacaktır. Bu durumda saniyeleri saymak için ‘x’ değişkeni, dakikaları saymak için ‘d’ değişkeni, saatleri saymak için ‘s’ değişkeni ve 15 dakikalık pompa döngüsü için ‘p değişkeni kullanılmaktadır.
 
Resim 3.2.1.7 Timer Interrupt Fonksiyonu'nun İçi
Kesme fonksiyonu içinde öncelikle ESP’den gelen epoch time bilgisi String olarak tarih ve saate dönüştürülmüştür. İnternet bağlantısı olmadığında epoch time verisi 1970 yılında kalmaktadır. Bu bilgiyi kullanarak if sorgusu ile internet bağlantısının kesildiği anlaşılmaktadır. Daha sonra timer içinde sayılan daika bilgisi 5 olana kadar pompa rölesi açık pozisyona getirilmektedir. Kalan 10 dakikada pompa rölesi kapatılmaktadır. Saat bilgisi ise 6’dan büyük ve 21’den küçük ise led rölesi açık hale getirilmektedir. Saat 6’dan küçük veya 21’den büyükse led rölesi kapalı hale getirilmektedir.
HC-SR04, 2 cm'den 400 cm'ye kadar 3mm hassasiyetle ölçüm yapabilen ultrasonik mesafe sensörüdür. Giriş pinine uygulanan 10 mikrosaniyelik TTL darbesi ve engelden çarpan ses dalgasının tekrar oluşturduğu TTL darbesi arasındaki süreyi ölçerek bu süreyi ses dalgasının hızı ile işleme sokar ve mesafe bilgisini verir. Fonksiyonda değişiklik yapılarak sensörün suya olan uzaklığı yerine su mesafesini elde edebilmek için haznenin yüksekliğinden sensörün okuduğu mesafe çıkarılmaktadır.
3.2.2	ESP 8266 NodeMCU Devresi
NodeMCU üzerinde ESP8266 modülü bulunduran açık kaynak kodlu, ufak boyutlu elektronik geliştirme kartıdır. Ucuz olmasına rağmen çok stabil çalışan bir karttır.  Kullanım alanı oldukça geniştir. Üzerinde bulunan ESP8266 Wifi modülü sayesinde internete kolay bir şekilde bağlanabilmekte ve bu özelliği sayesinde uzaktan kontrol ve IOT projelerinde çok fazla kullanılmaktadır. Ayrıca düşük güç tükettiği için, güç tüketimi önemli olan projelerde de çok tercih edilir. Normalde programla dili olarak Script’i kullanmaktadır. Aynı zaman da Arduino IDE üzerinden de programlanabildiği için Arduino ile aynı dilde programlanabilmektedir. NodeMCU aslında ESP8266’yı LUA dili ile programlamak için geliştirilmiş bir firmwarein ismidir. Fakat ayrıca NodeMcu DevKit isminde geliştirme kartları da olduğundan bu şekilde isimlendirilmektedir.

NodeMCU’yu Arduino IDE üzerinden programlamak için ESP8266 Modules kütüphanesinin tanımlanması gerekmektedir. WiFi bağlantısı ile Firebase sunucularına bağlanabilmek için de ‘ESP8266WiFi.h’ ve ‘FirebaseArduino.h’ kütüphanelerinin tanımlanması gerekmektedir. NTP (Network Time Protocol) sunucularına bağlanıp anlık zaman bilgisi alabilmek için ‘WiFiUdp.h’ ve ‘NTPClient.h’ kütüphanelerinin tanımlanması ve alınan epoch time bilgisini anlamlı tarih ve saate dönüştürmek için ‘TimeLib.h’ kütüphanesi tanımlanmalıdır. Ayrıca Arduino Uno ile seri haberleşme yapabilmek için ‘SoftwareSerial.h’ kütüphanesi tanımlanmalıdır. DS18B20 sıcaklık sensöründen veri okuyabilmk için de ‘OneWire.h’ ve ‘DallasTemperature.h’ kütüphaneleri tanımlanmalıdır.
ESP8266’yı wifi ile internete bağlamak ve Firebase Server’a bağlanabilmek için kodun başında gerekli değişkenler tanımlanmalıdır. Bunlar WiFi SSID yani bağlanılacak WiFi ağının ismi, WiFi Password ağın şifresi, Firebase Host sunucunun bize özel tanımladığı DNS adresi ve Firebase Auth sunucunun bize özel verdiği güvenlik tokeni.
Daha sonra kodun başlangıçta çalışan kısmı olan ‘void setup()’ içinde seri haberleşme, Wifi bağlantısı, Firebase bağlantısı ve DS18B20 bağlantısı kurulur.
DS18B20, su geçirmez sıcaklık sensörüdür. Dijital bir sensör olduğu için 1 metre uzunluğundaki kablosu yardımıyla derin mesafelerdeki sıcaklık rahatlıkla ölçülebilir ve data transferinde herhangi bir bozulma olmaz. Tek kablo (1-wire) arabirimi üzerinden 9 veya yapılandırılabilir 12 bitlik okuma gerçekleştirilebilir. Çeşitli bina içi, teçhizat, makine ve sıvı havuzlarının sıcaklık ölçümleri rahatlıkla yapılabilir. 
DS18B20 Sıcaklık Sensörü Özellikleri:
•	Çalışma Voltajı :  3.0V-5.0V
•	±0.5º C hassasiyetle -10º C-80º C arasında sıcaklık okuması
•	9 veya 12 bit seçilebilir çözünürlük
•	1-wire arabirimi ile iletişim için tek dijital pin kullanılabilir.
•	Sıcaklık limiti uyarı sistemi bulunmaktadır.
•	750 mS'den daha az bir tepki süresi
•	3 tel arayüzü:
•	Kırmızı Kablo  - VCC
•	Siyah Kablo  - GND
•	Beyaz Kablo  - DATA
•	6mm çapında 30mm boyunad paslanmaz çelik kılıf
•	1 metre uzunluğunda kablo
•	Su geçirmez
Bu sensörün iki çeşit bağlantı tipi vardır. Birinci bağlantı çeşidine ParaziteMode bağlantı adı verilir ve sensör enerjisini DQ(DATA) hattından Pull-Up direnci üzerinden alır ve sensörün VDD ve GND bacakları toprağa bağlıdır. Bunun avantajı daha az güç tüketmesidir.
İkinci bağlantı ise normal bağlantıdır. VDD bacağı +5V bağlıdır, GND bacağı toprağa yani Arduinonun GND’sine bağlıdır. DQ(DATA) bacağı da her iki bağlantıda olduğu gibi 4.7kOhm dirençle +5V’a bağlıdır bu direnç olmadan sensör ile Arduino arasında iletişim gerçekleşemez.
Devremizde normal bağlantı modu tercih edilmiş olup sensör çözünürlüğü de 12 bit çözünürlüğe ayarlanmıştır. Bu sayede daha hassas ölçüm değerleri alınabilmektedir.
Sistemde EC sensörü olarak Keystudio markasının TDS Meter V1.0 modelindeki analog TDS sensörü kullanılmıştır. Bu sensörde su geçirmez EC ölçüm probu, analog gerilim dönüştürücüsü ve mikrokontrolcü ile bağlantı yapılabilmesi için 3 adet dişi-erkek jumper kablo bulunmaktadır. 3 adet jumperdan biri +5VDC, diğeri GND diğeri de mikrokontrolcünün analog girişine bağlanmaktadır.
TDS Metre Özellikleri:
•	Giriş Gerilimi: DC 3.3 ~ 5.5V
•	Çıkış Gerilimi: 0 ~ 2.3V
•	Çalışma Akımı: 3 ~ 6mA
•	TDS Ölçüm Aralığı: 0 ~ 1000ppm
•	TDS Ölçüm Doğruluğu: ± 10% F.S. (25 ℃)
•	Modul Arayüzü: XH2.54-3P
•	Elektrot Arayüzü: XH2.54-2P

TDS Probu Özellikleri:
•	Uçtaki İğne Sayısı: 2
•	Kablo Uzunluğu: 60cm
•	Bağlantı Arayüzü: XH2.54-2P
•	Su Geçirmezlik: Su Geçirmez
Sensör enerjisini Arduino’nun 5VDC çıkışından almaktadır ancak Analog okumalar ESP8266 NodeMCU’nun A0 analog girişinden yapılmaktadır. Bu bağlantı biçiminde Arduino ve NodeMCU’nun GND’leri kısadevre edilmelidir. Bu şekilde bağlantı yapmamızın sebebi sensör verilerinin gerilim dalgalanmalarından etkilenmesini engellemek ve verileri NodeMCU ile doğrudan Firebase database’ine gönderebilmektir.
Bu kısımda TDS sensör analog pini, NodeMCU 3.3VDC ile çalıştırı için referans gerilimi, medyan dizisi sayısı ve gerekli değikenler tanımlanmıştır.
Medyan, bir dizinin tüm elemanlarını küçükten büyüğe sıralanması ve ortada bulunan sayının alınması işlemine denir. TDS sensörü, TDS okuma fonksiyonunun içine girdiğinde 40 milisaniye’den 800 milisaniye’ye kadar ölçülen verilerden en doğru 
olanını seçebilmek için 5 verinin ortasındaki veri medyan işlemi ile seçilmekte ve okunan TDS değeri bu olmaktadır.









TDS metreden gelen analog voltaj değerinin TDS değerine dönüştürülebilmesi için TDS metre fonksiyonu kullanılmaktadır. Bu fonksiyonda sıcaklık kompanzasyonu yapılabilmesi için DS18B20 sensöründen okunan sıcaklık kullanılmaktadır. Bu sayede
Sıcaklık değerinden etkilenmeksizin doğru TDS değeri okunabilmektedir. Ayrıca okunan TDS değeri, 0.5*1000 dönüştürme katsayısına bölünerek EC değeri de elde edilebilmektedir.
Sensörlerin doğruluğunu ölçmek için yapılan testlerde referans TDS Metre’de 701 PPM değeri okunurken ESP’ye bağlı analog TDS Metre’den 696.03 PPM değeri okunmuştur. Bu değerler arasında %0.71’lik bir sapma vardır. 
DS18B20 sıcaklık sensöründen su sıcaklığı değeri 14.25 °C okunmuştur. Referans sıcaklık sensöründen 14 °C okunmuştur. Bu değerler arasında %1.78’lik bir sapma vardır.
NTP Server’dan zaman bilgisi alabilmek için ‘pool.ntp.org’ domain adresine bağlanılmıştır. Türkiye’nin UTC zaman bölgesi UTC+3 olduğundan 3 saatin saniye karşılığı ‘utcOffset’ değişkeninde tanımlanmıştır.
NTP Servar’dan alınan epoch time verisinin anlamlı tarih ve saate dönüştürülebilmesi için ‘TimeLib’ kütüphanesi kullanılmıştır.
Verilerin Firebase ortamına aktarılması için başlarına güncel tarih ve saat bilgisi eklenmiştir. Daha sonra bu değerler ‘Firebase.pushString’ komutu ile database kaydı yapılırken ‘Firebase.setString’ komutu ile güncel değerlerinin Firebase arayüzünde 
daha kolay okunabilmesi sağlanmıştır.  
Google Firebase; web ve mobil uygulamalarının server tarafıyla geliştiricinin uğraşmasına gerek kalmadan kullanıcı giriş yetkilendirmeli ve verilerini gerçek zamanlı ve senkron bir şekilde tutulmasını sağlayan bir platformdur. Günümüzde ki projeler tüm markete hitap etmesi açısından iOS , Android ve web platformlarında geliştirilir fakat her platformun kendine ait yazılım dili ve bağlantı şekilleri vardır. Server-Side dediğimiz arkaplanda ki verilerin tutulması ve gerektiği zaman  kullanılması her platformun ortak sorunudur ve Google Firebase bu konuda geliştirilmiş ortak bir çözümdür. IOT kavramının altı doldurulurken en çok kullanılan terim  “bulut” terimidir. Bulut sistemlerinde veriler sistemimizin herhangi bir fiziksel parçasında depolanmaz, internet ortamında saklanırlar. Bu sayede veriler istenildiği anda istenilen noktadan ulaşılabilir , istenilen yerden değiştirilebilir durumdadır. Böylece gerçek manada, aralarındaki mesafeye bakılmaksızın bir nesneler ağı oluşturma imkanımız olur. Projemizin veri depolama miktarı Firebase’nin ücretsiz kullanım planına uymaktadır.
EC, sıcaklık ve TDS değerleri saniye başı kaydedilmektedir. Su seviyesi anlık olarak gösterilmektedir. LED ve pompa röleleri duruyor mu yoksa çalışıyor mu takip edilebilmektedir. Ayrıca LED ve pompa röleleri açma kapama ve zaman ayarına bırakma işlemleri yapılabilmektedir.
Firebase databasede kaydedilen veriler ‘JSON’ formatında indirilip ‘CSV’ dosya formatına dönüştürerek Microsoft Excel programında işlenebilmektedir.
