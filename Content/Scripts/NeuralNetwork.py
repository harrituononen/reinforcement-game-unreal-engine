import tensorflow as tf
import tensorflow.contrib.slim as slim
import numpy as np
import json
import unreal_engine as ue
import random
import math
import upypip as pip
from TFPluginAPI import TFPluginAPI

class Memory:
	def __init__(self, max_memory):
		self.max_memory = max_memory
		self.samples = []
	
	def add(self, sample):
		self.samples.append(sample)
		if len(self.samples) > self.max_memory:
			self.samples.pop(0)
	
	def sample(self, no_samples):
		if no_samples > len(self.samples):
			return random.sample(self.samples, len(self.samples))
		else:
			return random.sample(self.samples, no_samples)
		
class Model:
	def __init__(self, sess, num_states, num_actions):
		self.sess = sess
		self.num_states = num_states
		self.num_actions = num_actions
		self.learning_rate = 0.00001

		self.states = None
		self.actions = None
		self.output = None
		self.optimizer = None
		self.create_model()
	
	def create_model(self):
		self.states = tf.placeholder(shape=[None,self.num_states],dtype=tf.float32)
		self.q_values = tf.placeholder(shape=[None,self.num_actions],dtype=tf.float32)
		
		layer1 = tf.layers.dense(self.states,300,activation=tf.nn.relu)
		
		layer2 = tf.layers.dense(layer1,300,activation=tf.nn.relu)
		layer3 = tf.layers.dense(layer2,300,activation=tf.nn.relu)

		self.output = tf.layers.dense(layer3,self.num_actions)
		
		loss = tf.losses.mean_squared_error(self.q_values,self.output)
		self.optimizer = tf.train.AdamOptimizer(learning_rate=self.learning_rate).minimize(loss)
	
	def predict_one(self, state, sess):
		return sess.run(self.output, feed_dict={self.states:state.reshape(1, self.num_states)})
	
	def predict_batch(self, states, sess):
		return sess.run(self.output, feed_dict={self.states: states})
	
	def train_batch(self, sess, new_states_batch, new_q_values_batch):
		sess.run(self.optimizer, feed_dict={self.states:new_states_batch,self.q_values:new_q_values_batch})

class NeuralNetwork(TFPluginAPI):
	#
	# BeginTraining -> Start -> EGetState -> CFTakeAction -> ETakeAStep -> CFSendAllData -> CFTakeAction
	#
	def onBeginTraining(self):
		self.num_states = 6
		self.num_actions = 4
		self.mini_batch_size = 50
		self.batch_size = 500
		self.gamma = 0.99
		self.epsilon = 1.0
		self.episodes = 0
		self.steps = 0
		self.reward_store = []
		self.total_reward = 0
		self.model = None
		self.memory = Memory(50000)
		self.sess = tf.Session()
		self.model = Model(self.sess, self.num_states, self.num_actions)
		self.sess.run(tf.global_variables_initializer())
		self.start('')
		
	def endSession(self, jsonInput):
		self.sess.close()
		
	def start(self, _):
		self.callEvent('GetStateEvent', None, True)
		
	def takeAction(self, jsonInput):
		input = json.loads(jsonInput)
		state_array = self.getStateArray(
										# input["myX"]
										# , input["myY"]
										# , input["enemyX"]
										# , input["enemyY"]
										 input["myLocation"]
										, input["enemyLocation"]
										, input["closestProjectileLocation"]
										 , input["secondClosestProjectileLocation"]
										 , input["thirdClosestProjectileLocation"]
										, input["closestProjectileLocationToEnemy"]
										#, input["hittingShot"]
										, input["lastShot"]
										 , input["enemyXVelocity"]
										 , input["enemyXVelocity"]
										# , input["distanceToEnemyX"]
										# , input["shotBulletsInPlay"]
										# , input["bBulletHeadingTowardsEnemy"]
										# , input["bBulletInFront"]
										# , input["bBulletBehind"]
										# , input["bBulletLeft"]
										# , input["bBulletRight"]
										# , input["bBulletInFrontOfEnemy"]
										# , input["bBulletBehindEnemy"]
										# , input["bBulletLeftOfEnemy"]
										# , input["bBulletRightOfEnemy"]
										# , input["bMovingUp"]
										# , input["bMovingDown"]
										# , input["bMovingLeft"]
										# , input["bMovingRight"]
										# , input["bEnemyMovingUp"]
										# , input["bEnemyMovingDown"]
										# , input["bEnemyMovingLeft"]
										# , input["bEnemyMovingRight"]
										# , input["bTooFarAway"]
										 , input["bEnemyLeft"]
										 , input["bEnemyRight"]
										# , input["bEnemyFired"]
										# , input["bHitFront"]
										# , input["bHitBehind"]
										# , input["bHitLeft"]
										# , input["bHitRight"]
										# , input["bCloseToTopWall"]
										# , input["bCloseToBottomWall"]
										# , input["bCloseToMiddleWall"]
										# , input["bCloseToRightWall"]
										# , input["bCloseToLeftWall"]
										 , input["bCanFire"]
										)
		action = self.choose_action(state_array)
		action_in_json = { "action": action }
		self.callEvent('TakeAStepEvent', action_in_json, True)
		
	def sendNetworkData(self, jsonInput):
		input = json.loads(jsonInput)
		state = input['data']['state']
		action = input['data']['action']
		reward = input['data']['reward']
		new_state = input['data']['newState']
		done = input['data']['bDone']
		max_episode_length = input['data']['maxEpisodeLength']
		state_array = self.getStateArray(
										# state["myX"]
										# , state["myY"]
										# , state["enemyX"]
										# , state["enemyY"]
										 state["myLocation"]
										, state["enemyLocation"]
										, state["closestProjectileLocation"]
										 , state["secondClosestProjectileLocation"]
										, state["thirdClosestProjectileLocation"]
										, state["closestProjectileLocationToEnemy"]
										#, state["hittingShot"]
										, state["lastShot"]
										 , state["enemyXVelocity"]
										 , state["enemyYVelocity"]
										# , state["distanceToEnemyX"]
										# , state["shotBulletsInPlay"]
										# , state["bBulletHeadingTowardsEnemy"]
										# , state["bBulletInFront"]
										# , state["bBulletBehind"]
										# , state["bBulletLeft"]
										# , state["bBulletRight"]
										# , state["bBulletInFrontOfEnemy"]
										# , state["bBulletBehindEnemy"]
										# , state["bBulletLeftOfEnemy"]
										# , state["bBulletRightOfEnemy"]
										# , state["bMovingUp"]
										# , state["bMovingDown"]
										# , state["bMovingLeft"]
										# , state["bMovingRight"]
										# , state["bEnemyMovingUp"]
										# , state["bEnemyMovingDown"]
										# , state["bEnemyMovingLeft"]
										# , state["bEnemyMovingRight"]
										# , state["bTooFarAway"]
										 , state["bEnemyLeft"]
										 , state["bEnemyRight"]
										# , state["bEnemyFired"]
										# , state["bHitFront"]
										# , state["bHitBehind"]
										# , state["bHitLeft"]
										# , state["bHitRight"]
										# , state["bCloseToTopWall"]
										# , state["bCloseToBottomWall"]
										# , state["bCloseToMiddleWall"]
										# , state["bCloseToRightWall"]
										# , state["bCloseToLeftWall"]
										 , state["bCanFire"]
										)
										
		new_state_array = self.getStateArray(
											# new_state["myX"]
											# , new_state["myY"]
											# , new_state["enemyX"]
											# , new_state["enemyY"]
											 new_state["myLocation"]
											, new_state["enemyLocation"]
											, new_state["closestProjectileLocation"]
											 , new_state["secondClosestProjectileLocation"]
											 , new_state["thirdClosestProjectileLocation"]
											, new_state["closestProjectileLocationToEnemy"]
											#, new_state["hittingShot"]
											, new_state["lastShot"]
											 , new_state["enemyXVelocity"]
											 , new_state["enemyYVelocity"]
											# , new_state["distanceToEnemyX"]
											# , new_state["shotBulletsInPlay"]
											# , new_state["bBulletHeadingTowardsEnemy"]
											# , new_state["bBulletInFront"]
											# , new_state["bBulletBehind"]
											# , new_state["bBulletLeft"]
											# , new_state["bBulletRight"]
											# , new_state["bBulletInFrontOfEnemy"]
											# , new_state["bBulletBehindEnemy"]
											# , new_state["bBulletLeftOfEnemy"]
											# , new_state["bBulletRightOfEnemy"]
											# , new_state["bMovingUp"]
											# , new_state["bMovingDown"]
											# , new_state["bMovingLeft"]
											# , new_state["bMovingRight"]
											# , new_state["bEnemyMovingUp"]
											# , new_state["bEnemyMovingDown"]
											# , new_state["bEnemyMovingLeft"]
											# , new_state["bEnemyMovingRight"]
											# , new_state["bTooFarAway"]
											 , new_state["bEnemyLeft"]
											 , new_state["bEnemyRight"]
											# , new_state["bEnemyFired"]
											# , new_state["bHitFront"]
											# , new_state["bHitBehind"]
											# , new_state["bHitLeft"]
											# , new_state["bHitRight"]
											# , new_state["bCloseToTopWall"]
											# , new_state["bCloseToBottomWall"]
											# , new_state["bCloseToMiddleWall"]
											# , new_state["bCloseToRightWall"]
											# , new_state["bCloseToLeftWall"]
											 , new_state["bCanFire"]
											)
											

		self.total_reward += reward
		self.steps += 1
		if done == True:
			self.episodes += 1
			if self.episodes < 50:
				self.epsilon = 1.0
			else:
				self.epsilon = 0
			self.reward_store.append(self.total_reward)
			self.total_reward = 0
			if max_episode_length < 20000:
				self.memory.add((state_array, action, reward, new_state_array, done))
				self.replay(self.batch_size)
			self.callEvent('ResetGameEvent', None, True)
		else:
			if max_episode_length < 20000:
				self.memory.add((state_array, action, reward, new_state_array, done))
				self.replay(self.mini_batch_size)
			next_state = json.dumps(new_state)
			self.takeAction(next_state)
	
	def getStateArray(self
						#, myX
						#, myY
						#, enemyX
						#, enemyY
						, myLocation
						, enemyLocation
						, closestProjectileLocation
						, secondClosestProjectileLocation
						, thirdClosestProjectileLocation
						, closestProjectileLocationToEnemy
						#, hittingShot
						, lastShot
						 , enemyXVelocity
						 , enemyYVelocity
						# , distanceToEnemyX
						# , shotBulletsInPlay
						# , bBulletHeadingTowardsEnemy
						# , bBulletInFront
						# , bBulletBehind
						# , bBulletLeft
						# , bBulletRight
						# , bBulletInFrontOfEnemy
						# , bBulletBehindEnemy
						# , bBulletLeftOfEnemy
						# , bBulletRightOfEnemy
						# , bMovingUp
						# , bMovingDown
						# , bMovingLeft
						# , bMovingRight
						# , bEnemyMovingUp
						# , bEnemyMovingDown
						# , bEnemyMovingLeft
						# , bEnemyMovingRight
						# , bTooFarAway
						 , bEnemyLeft
						 , bEnemyRight
						# , bEnemyFired
						# , bHitFront
						# , bHitBehind
						# , bHitLeft
						# , bHitRight
						# , bCloseToTopWall
						# , bCloseToBottomWall
						# , bCloseToMiddleWall
						# , bCloseToRightWall
						# , bCloseToLeftWall
						 , bCanFire
						):
		array = np.array([
						myLocation["x"], myLocation["y"]
						#, enemyLocation["x"], enemyLocation["y"]
						, closestProjectileLocation["x"], closestProjectileLocation["y"]
						, secondClosestProjectileLocation["x"], secondClosestProjectileLocation["y"]
						#, thirdClosestProjectileLocation["x"], thirdClosestProjectileLocation["y"]
						#, closestProjectileLocationToEnemy["x"], closestProjectileLocationToEnemy["y"]
						#, hittingShot["x"], hittingShot["y"]
						#, lastShot["x"], lastShot["y"]
						#, enemyXVelocity, enemyYVelocity
						#, enemyLocation, projectileLocations
						#, bBulletInFront, bBulletBehind, bBulletLeft, bBulletRight
						#, bMovingUp, bMovingDown, bMovingLeft, bMovingRight
						#, bEnemyMovingUp, bEnemyMovingDown, bEnemyMovingLeft, bEnemyMovingRight
						#, bEnemyLeft, bEnemyRight
						#, bHitFront, bHitBehind, bHitLeft, bHitRight
						#, bCloseToTopWall, bCloseToMiddleWall, bCloseToRightWall, bCloseToLeftWall
						#, bCanFire
						])
		return array.astype(float)
		
	def choose_action(self,state):
		if random.random() < self.epsilon:
			return random.randint(0, self.model.num_actions - 1)
		else:
			return float(np.argmax(self.model.predict_one(state,self.sess)))
	
	def replay(self, batch_size):
		batch = self.memory.sample(batch_size)
		states = np.array([i[0] for i in batch])
		next_states = np.array([i[3] for i in batch])
		q_values = self.model.predict_batch(states, self.sess)
		q_values_next = self.model.predict_batch(next_states, self.sess)
		new_states = np.zeros((len(batch), self.model.num_states))
		new_q_values = np.zeros((len(batch), self.model.num_actions))
		for i, val in enumerate(batch):
			state, action, reward, next_state, done = val[0], val[1], val[2], val[3], val[4]
			q = q_values[i]
			if done:
				q[action] = reward
			else:
				q[action] = reward + self.gamma * np.amax(q_values_next[i])
			new_states[i] = state
			new_q_values[i] = q
		self.model.train_batch(self.sess, new_states, new_q_values)
	
	def onSetup(self):
		pass
		
	def onJsonInput(self, jsonInput):
		pass

	def initNetwork(self, jsonInput):
		pass
		
def getApi():
	return NeuralNetwork.getInstance()