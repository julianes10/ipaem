import logging
import sys
import os
import helper

from helper import *
from flask import Flask, jsonify,abort,make_response,request, url_for


tasks = [
    {
        'id': 1,
        'title': u'Buy groceries',
        'description': u'Milk, Cheese, Pizza, Fruit, Tylenol', 
        'done': False
    },
    {
        'id': 2,
        'title': u'Learn Python',
        'description': u'Need to find a good Python tutorial on the web', 
        'done': False
    }
]

api = Flask("api")


def make_public_task(task):
    new_task = {}
    for field in task:
        if field == 'id':
            new_task['uri'] = url_for('get_tasks', task_id=task['id'], _external=True)
        else:
            new_task[field] = task[field]
    return new_task

@api.route('/ipaem/api/v1.0/tasks', methods=['GET'])
def get_tasks():
    global ga
    if ga.getBusy()==False:
      helper.internalLogger.debug("fake trigger NOW")
      assistant.start_conversation()
    else:
      helper.internalLogger.debug("fake trigger not NOW, conversation ongoing")
    return jsonify({'tasks': [make_public_task(task) for task in tasks]})


