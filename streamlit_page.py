import streamlit as st
import sqlalchemy as db
import asyncio

engine = db.create_engine("sqlite:///users_database.db")
con = engine.connect()
metadata = db.MetaData()
authorized_users = db.Table("authorized_users", metadata, autoload_with=engine)
access_history = db.Table("access_history", metadata, autoload_with=engine)


def check_valid_key(str_key):
    try:
        keys = str_key.split(" ")
        for key in keys:
            if int(key) > 255:
                return False
    except ValueError:
        return False

    return True


async def update_db():
    while True:
        users_data = con.execute(db.select(authorized_users)).fetchall()
        users_table.table(users_data)
        history_data = con.execute(db.select(access_history)).fetchall()
        history_table.table(history_data)
        await asyncio.sleep(2)


st.write("### :blue[Користувачі які мають доступ]")
users_table = st.empty()

st.write("### :blue[Додати нового користувача в базу даних]")
new_name = st.text_input("Введіть ім'я нового користувача")
new_key = st.text_input(
    "Введіть ключ нового користувача по шаблону (без лапок): '255 255 255 255'"
)
add_user_btn = st.button("Додати")
if add_user_btn:
    if new_name == "" or new_key == "":
        st.write(":red[Введіть дані в поля вище]")
    elif not check_valid_key(new_key):
        st.write(":red[Невірно введений ключ]")
    else:
        query = db.insert(authorized_users).values(name=new_name, key=new_key)
        con.execute(query)
        con.commit()
        st.write(":green[Додано]")


st.write("### :blue[Видалити користувача з бази даних]")
id_to_delete = st.text_input("Введіть id користувача якого ви бажаєте видалити:")
delete_user_btn = st.button("Видалити")

if delete_user_btn:
    if id_to_delete == "":
        st.write(":red[Введіть дані в поле вище]")
    else:
        query = db.delete(authorized_users).where(
            authorized_users.c.id == int((id_to_delete))
        )
        con.execute(query)
        con.commit()
        st.write(":green[Видалено]")


st.write("### :blue[Історія користуванням дверима]")
history_table = st.empty()

st.write("### :blue[Додати запис про користування дверима]")
new_history_key = st.text_input("Введіть ключ користувача який користувався дверима")
add_history_btn = st.button("Додати запис")

if add_history_btn:
    if new_history_key == "":
        st.write(":red[Введіть дані в поле вище]")
    elif not check_valid_key(new_history_key):
        st.write(":red[Невірно введений ключ]")
    else:
        query = db.insert(access_history).values(key=new_history_key)
        con.execute(query)
        con.commit()
        st.write(":green[Додано]")


st.write("### :blue[Видалити запис про користування дверима]")
delete_history_id = st.text_input("Введіть id запису який потрібно видалити")
delete_history_btn = st.button("Видалити запис")

if delete_history_btn:
    if delete_history_id != "":
        query = db.delete(access_history).where(
            access_history.c.id == delete_history_id
        )
        con.execute(query)
        con.commit()
        st.write(":green[Видалено]")
    else:
        st.write(":red[Введіть дані в поле вище]")

asyncio.run(update_db())
