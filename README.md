# ESP8266---Rain-detect-and-Notify-with-rain-radar-map.
ตรวจจับน้ำฝนและแจ้งเตือนเข้า Line<br>
- จะไม่มีการแจ้งซ้ำ ๆ ทุกครั้งที่ตรวจจับเจอ เพราะใช้เงื่อนไขเยอะ<br>
- ใช้การหาค่าเฉลี่ย 1-3 นาที เพื่อความแม่นยำ แต่ก็อาจจะทำให้แจ้งเตือนได้ช้ากว่าฝนที่กำลังตกนิดหน่อย<br>
- เมื่อค่าของเซนเซอร์มีการเปลี่ยนแปลงนิดหน่อย และใน 3 นาทีต่อมา ถ้าค่ายังเปลี่ยนแปลงไม่กลับไปเหมือนเดิม จะส่งแจ้งเตือนบอกว่าฝนอาจจะตกเบา ๆ ปอย ๆ หรือ มีอะไรไปติดอยู่ที่เซนเซฮร์ (รวมถึงส่งแผนที่เรดาร์ฝนให้ตรวจสอบ ถ้ามีการเปิดใช้งานส่วนนี้)<br>
- แต่ถ้าฝนตกหนักกว่าการตกเบา ๆ ปอย ๆ จะข้ามการแจ้งเตือนด้านบนไป และจะแจ้งว่าตรวจเจอน้ำฝน คาดว่าฝนกำลังตก (รวมถึงส่งแผนที่เรดาร์ฝนให้ตรวจสอบ ถ้ามีการเปิดใช้งานส่วนนี้)<br>
- เมื่อฝนตกแรงขึ้นกว่าเดิม จะแจ้งว่าพบน้ำฝนมากขึ้น คาดว่าฝนตกหนักขึ้น<br>
- ถ้าฝนตกหนักกว่าเดิม จะแจ้งว่าฝนกำลังตกหนักมาก<br>
- ถ้าตกหนักกว่าเดิมอีก จะแจ้งว่าตกโคตรหนัก<br>
- เมื่อฝนเริ่มตกเบาลง ก็จะแจ้งว่าฝนตกเบาลงจากเดิม จากระดับนั้น ๆ<br>
- ถ้าเบาลงกว่าเดิมเยอะ จนแทบจะกลับไปค่าเริ่มต้น ก็จะแจ้งว่าฝนเริ่มซา<br>
- แต่ถ้าจากฝนซาแล้วกลับมาตกแรงขึ้น ก็จะแจ้งว่าฝนกลับมาตกแรงขึ้น<br>
- เมื่อฝนแทบจะไม่ตกแล้ว ก็จะแจ้งว่า ฝนอาจจะตกเบา ๆ อาจจะตกไปแบบนี้เรื่อย ๆ หรืออาจจะหยุดตกแล้ว หรือไม่ตกแล้ว<br>
- ถ้ากลับไปค่าเริ่มต้น ก็จะแจ้งว่า ฝนหยุดตกโดยสมบูรณ์ซํกระยะแล้ว (รวมถึงส่งแผนที่เรดาร์ฝนให้ตรวจสอบ ถ้ามีการเปิดใช้งานส่วนนี้)<br>
- ส่งค่าปัจจุบันและค่าเฉลี่ยของเซนเซอร์ทุก 30 นาที เพื่อไว้ตรวจสอบการทำงาน<br>
<br>
**ระบบส่งแผนที่ฝนเข้า Line ต้องมี PHP/Web Server ด้วย<br>
ดูรายละเอียดและ Script ได้ที่ https://github.com/superogira/php---Bangkok-rain-radar-line-notify<br>
<br>
**รายละเอียดอื่น ๆ เข้าไปอ่านในกลุ่ม Facebook ได้ที่ - https://www.facebook.com/groups/arduino.thai/permalink/3241545302554891/
