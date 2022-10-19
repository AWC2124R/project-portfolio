import requests
import cv2
import time
import requests
import numpy

from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from webdriver_manager.chrome import ChromeDriverManager


def search_google(search_query):
  chrome_options = webdriver.ChromeOptions() 
  browser = webdriver.Chrome("chromedriver.exe")

  search_url = f"https://www.google.com/search?site=&tbm=isch&source=hp&biw=1873&bih=990&q={search_query}"
  images_url = []

  browser.get(search_url)
  elements = browser.find_element(By.CSS_SELECTOR, 'body')

  for i in range(20):
    elements.send_keys(Keys.PAGE_DOWN)
    time.sleep(1)
  
  count = 0
  temp = browser.find_elements(By.CSS_SELECTOR, 'a > div > img')
  for img in temp:
    link = img.get_attribute("src")
    if "http" in link:
      images_url.append(link)
      count += 1
    if count == 400:
      break

  return images_url

items = search_google("Fire News")

image_arr_color = []
for i in items:
  image_nparray = numpy.asarray(bytearray(requests.get(i).content), dtype=numpy.uint8)
  image = cv2.imdecode(image_nparray, cv2.IMREAD_COLOR)
  image_arr_color.append(image)
  cv2.waitKey(0)

print(len(image_arr_color))

for i in range(len(image_arr_color)):
  if not cv2.imwrite("Fire_Crawl/Image_(%s).jpg" % str(i), image_arr_color[i]):
    print("Can't Save")
