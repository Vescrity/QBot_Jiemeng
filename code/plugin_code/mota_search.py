import requests
import sys, io
sys.stdout = io.TextIOWrapper(sys.stdout.buffer,encoding='utf8')
from requests.models import Response


def test_request(arg) -> Response:
    fields = {
        "sortmode": (None, "hotest"),
        "colormode": (None, "#"),
        "searchstr": (None, arg),
        "page": (None, 1),
        "tags": (None, "[]"),
    }
    resp = requests.post(url='https://h5mota.com/backend/towers/list.php', files=fields)
    return resp


if __name__ == "__main__":
    fp=open("Current_msg.txt","r",encoding='utf-8')
    p=fp.read()
    p=p[p.find('#')+1:]
    resp = test_request(p)
    resp_ob = resp.json()
    resp_ob=resp_ob["towers"]
    n=len(resp_ob)
    if n>5:
        n=5
    all_ans=""
    for i in range(n):
        resp_obj=resp_ob[i]
        ans_form="\n塔名：%s(%s)\n作者：%s(%s)\n评分：%s 难度：%s\n层数：%s\n"%(resp_obj["title"],resp_obj["name"],resp_obj["author"],resp_obj["authorId"],resp_obj["rate"],resp_obj["difficultrate"],resp_obj["floors"])
        all_ans=all_ans+ans_form
    print(all_ans)